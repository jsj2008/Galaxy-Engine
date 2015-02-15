/*!
 * \file scenemanager.cpp
 * \brief Manage the scene
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "scenemanager.h"

using namespace std;
using namespace glm;
namespace GXY
{
    SceneManager::SceneManager(void)
    {
        mRootNode = make_shared<Node>(mat4(1.0f));

        mGeometryFrameBuffer = make_shared<FrameBuffer>();

        mGeometryFrameBuffer->create();
        mGeometryFrameBuffer->createTexture(powerOf2(global->device->width()), powerOf2(global->device->height()),
                                            {RGB8_UNORM, RGBA32F, RGBA16F, RGB16F}, true, false);
    }

    void SceneManager::createCameraStatic(const vec3 &pos, const vec3 &look)
    {
        mCamera = make_shared<CameraStatic>(pos, look, CAM_UP_Y,
                                            radians(45.0f), (float)global->device->width() / global->device->height(), 1.0, 10000.0f);
    }

    void SceneManager::createCameraFPS(const vec3 &pos, float speed, float sensitivity)
    {
        mCamera = make_shared<CameraFPS>(pos, CAM_UP_Y, speed, sensitivity,
                                         radians(45.0f), (float)global->device->width() / global->device->height(), 1.0, 10000.0f);
    }

    void SceneManager::render()
    {
        initialize();

        renderModels();

        renderFinal();
    }

    void SceneManager::initialize()
    {
        mCamera->update();

        global->Model.command->setToZeroElement();
        global->Model.toWorldSpace->setToZeroElement();

        global->Uniform.contextBuffer->map()->frustrumMatrix = mCamera->toClipSpace();

        for(u32 i = 0; i < 6; ++i)
            global->Uniform.contextBuffer->map()->planesFrustrum[i] = mCamera->frustrum().mPlanes[i].plane;
    }

    void SceneManager::renderModels()
    {
        mGeometryFrameBuffer->bind();
            global->device->clearDepthColorBuffer();

            mRootNode->pushModelsInPipeline(mCamera->frustrum());
            global->Uniform.contextBuffer->map()->mNumberMeshes.x = global->Model.command->numElements();

            // Compute Matrix and Culling pass
            global->Shaders.matrixCulling->use();
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_UNIFORM_BARRIER_BIT | GL_COMMAND_BARRIER_BIT);
                glDispatchCompute(global->Model.command->numElements() / 128 + 1, 1, 1);

            //Depth Pass
            global->Model.command->bind(DRAW_INDIRECT);
            global->Shaders.depth->use();
            global->Model.vaoDepth->bind();
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

                synchronize(); // clear and dispatch
                glMemoryBarrier(GL_COMMAND_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
                glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, global->Model.command->numElements(), 0);

            // Rendering pass
            global->Shaders.model->use();
            global->Model.vao->bind();
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthFunc(GL_EQUAL);
            glDepthMask(GL_FALSE);
                glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, global->Model.command->numElements(), 0);
            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
    }

    void SceneManager::renderFinal()
    {
        global->Shaders.final->use();
        global->Quad.vao->bind();

        synchronize();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        mGeometryFrameBuffer->bindTextures(0, 0, 1);
        global->device->setViewPort();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
