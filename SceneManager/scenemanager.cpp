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
        global->sceneManager = this;
        mRootNode = make_shared<Node>(mat4(1.0f));

        mGeometryFrameBuffer = make_shared<FrameBuffer>();
        mDirectLightFrameBuffer = make_shared<FrameBuffer>();

        mGeometryFrameBuffer->create();
        mDirectLightFrameBuffer->create();

        mGeometryFrameBuffer->createTexture(powerOf2(global->device->width()), powerOf2(global->device->height()),
                                            {RGB8_UNORM, RGB32F, RGB32F, RG32F}, true);

        mDirectLightFrameBuffer->createTexture(powerOf2(global->device->width()), powerOf2(global->device->height()), {RGB16F}, false);

        mImageAmbientOcclusion = make_shared<Texture>(3);

        for(u32 i = 0; i < 3; ++i)
            mImageAmbientOcclusion->emptyTexture(i, powerOf2(global->device->width()), powerOf2(global->device->height()), R32F);
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

        mGeometryFrameBuffer->bind();
        global->device->clearDepthColorBuffer();
            pushModelsInPipeline(mCamera);

            global->Shaders.depth->use();
                renderDepthPass();
            global->Shaders.model->use();
                renderModels();

        renderAmbientOcclusion();

            renderPointLights();
        glDisable(GL_BLEND);

        renderFinal();
    }

    void SceneManager::initialize()
    {
        mCamera->update();

        global->Uniform.contextBuffer->map()->inverseSizeFrameBufferAO = vec4(1.0f) / vec4(powerOf2(global->device->width()), powerOf2(global->device->height()),
                                                                                           powerOf2(global->device->width()), powerOf2(global->device->height()));
    }

    void SceneManager::pushModelsInPipeline(shared_ptr<AbstractCamera> const &camera)
    {
        global->Model.command->setToZeroElement();
        global->Model.toWorldSpace->setToZeroElement();

        global->Uniform.frustrumBuffer->map()->frustrumMatrix = camera->toClipSpace();
        global->Uniform.frustrumBuffer->map()->posCamera = camera->position();
        for(u32 i = 0; i < 6; ++i)
            global->Uniform.frustrumBuffer->map()->planesFrustrum[i] = camera->frustrum().mPlanes[i].plane;

            mRootNode->pushModelsInPipeline(camera->frustrum());
            global->Uniform.frustrumBuffer->map()->numberMeshesPointLights.x = global->Model.command->numElements();

            // Compute Matrix and Culling pass
            global->Shaders.matrixCulling->use();
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_UNIFORM_BARRIER_BIT | GL_COMMAND_BARRIER_BIT);
                glDispatchCompute(global->Model.command->numElements() / 64 + 1, 1, 1);
    }

    void SceneManager::renderDepthPass()
    {
        //Depth Pass
        global->Model.command->bind(DRAW_INDIRECT);
        global->Model.vaoDepth->bind();
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            synchronize(); // clear and dispatch
            glMemoryBarrier(GL_COMMAND_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, global->Model.command->numElements(), 0);
    }

    void SceneManager::renderModels()
    {
        // Rendering pass
        global->Model.vao->bind();
        synchronize();
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthFunc(GL_EQUAL);
        glDepthMask(GL_FALSE);
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, global->Model.command->numElements(), 0);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }

    void SceneManager::renderPointLights()
    {
        global->Lighting.commandPointLights->setToZeroElement();
        global->Lighting.pointLight->setToZeroElement();
        global->Lighting.toWorldSpace->setToZeroElement();

        mRootNode->pushPointLightsInPipeline();

        global->Uniform.frustrumBuffer->map()->frustrumMatrix = mCamera->toClipSpace();
        global->Uniform.frustrumBuffer->map()->posCamera = mCamera->position();
        for(u32 i = 0; i < 6; ++i)
            global->Uniform.frustrumBuffer->map()->planesFrustrum[i] = mCamera->frustrum().mPlanes[i].plane;

        global->Uniform.frustrumBuffer->map()->numberMeshesPointLights.y = global->Lighting.commandPointLights->numElements();

        global->Shaders.projectPointLights->use();
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_UNIFORM_BARRIER_BIT);

        glDispatchCompute(global->Lighting.commandPointLights->numElements() / 64 + 1, 1, 1);

        global->Shaders.computePointLights->use();
        global->Lighting.vaoPointLight->bind();
        global->Lighting.commandPointLights->bind(DRAW_INDIRECT);
        mGeometryFrameBuffer->bindTextures(1, 0, 2);
        mGeometryFrameBuffer->bindTextures(3, 2, 1);
        global->Lighting.pointLightShadowMaps->bindTextures(0, 3, 1);

        mDirectLightFrameBuffer->bind();
        global->device->clearColorBuffer();
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        synchronize();
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_COMMAND_BARRIER_BIT);
        glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, global->Lighting.commandPointLights->numElements(), 0);
    }

    void SceneManager::renderAmbientOcclusion()
    {
        mImageAmbientOcclusion->bindImages(0, 0, 1);
        mGeometryFrameBuffer->bindTextures(1, 0, 2);
        global->Shaders.ambientOcclusion->use();

            synchronize();
            glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
            glDispatchCompute(powerOf2(global->device->width()) / 8, powerOf2(global->device->height()) / 8, 1);

        mImageAmbientOcclusion->bindImages(1, 0, 1);
        mImageAmbientOcclusion->bindTextures(0, 0, 1);
        global->Shaders.blurHorizontalPass->use();

            synchronize();
            glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
            glDispatchCompute(powerOf2(global->device->width()) / 64, powerOf2(global->device->height()), 1);

        mImageAmbientOcclusion->bindImages(2, 0, 1);
        mImageAmbientOcclusion->bindTextures(1, 0, 1);
        global->Shaders.blurVerticalPass->use();
            synchronize();
            glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
            glDispatchCompute(powerOf2(global->device->width()), powerOf2(global->device->height()) / 64, 1);
    }

    void SceneManager::renderFinal()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        global->Shaders.final->use();
        global->Quad.vao->bind();

        mGeometryFrameBuffer->bindTextures(0, 0, 1);
        mImageAmbientOcclusion->bindTextures(2, 1, 1);
        mDirectLightFrameBuffer->bindTextures(0, 2, 1);
        global->device->setViewPort();

            synchronize();
            glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
