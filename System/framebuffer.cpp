/*!
 * \file framebuffer.cpp
 * \brief Create Frame Buffer Object
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "framebuffer.h"
#include "device.h"
#include "../SceneManager/scenemanager.h"

namespace GXY
{
using namespace std;
using namespace glm;

static GLenum attachments[] = {GL_COLOR_ATTACHMENT0,
                               GL_COLOR_ATTACHMENT1,
                               GL_COLOR_ATTACHMENT2,
                               GL_COLOR_ATTACHMENT3,
                               GL_COLOR_ATTACHMENT4,
                               GL_COLOR_ATTACHMENT5,
                               GL_COLOR_ATTACHMENT6,
                               GL_COLOR_ATTACHMENT7,
                               GL_COLOR_ATTACHMENT8,
                               GL_COLOR_ATTACHMENT9,
                               GL_COLOR_ATTACHMENT10,
                               GL_COLOR_ATTACHMENT11,
                               GL_COLOR_ATTACHMENT12,
                               GL_COLOR_ATTACHMENT13,
                               GL_COLOR_ATTACHMENT14,
                               GL_COLOR_ATTACHMENT15
                              };

static vec3 const look[] = {vec3(1.0, 0.0, 0.0),
                            vec3(-1.0, 0.0, 0.0),
                            vec3(0.0, 1.0, 0.0),
                            vec3(0.0, -1.0, 0.0),
                            vec3(0.0, 0.0, 1.0),
                            vec3(0.0, 0.0, -1.0)};

static CameraUp const up[] = {CAM_DOWN_Y,
                              CAM_DOWN_Y,
                              CAM_UP_Z,
                              CAM_DOWN_Z,
                              CAM_DOWN_Y,
                              CAM_DOWN_Y};

    FrameBuffer::FrameBuffer(void) : mId(0), mNumber(0), mW(0), mH(0)
    {

    }

    FrameBuffer::FrameBuffer(FrameBuffer &&frameBuffer)
    {
        mId = std::move(frameBuffer.mId);
        mH = std::move(frameBuffer.mH);
        mW = std::move(frameBuffer.mW);
        mNumber = std::move(frameBuffer.mNumber);
        mColorBuffer = std::move(frameBuffer.mColorBuffer);
        mDepthBuffer = std::move(frameBuffer.mDepthBuffer);

        frameBuffer.mId = frameBuffer.mH = frameBuffer.mW = frameBuffer.mNumber = 0;
        frameBuffer.mColorBuffer.destroy();
        frameBuffer.mDepthBuffer.destroy();
    }

    FrameBuffer &FrameBuffer::operator=(FrameBuffer &&frameBuffer)
    {
        mId = std::move(frameBuffer.mId);
        mH = std::move(frameBuffer.mH);
        mW = std::move(frameBuffer.mW);
        mNumber = std::move(frameBuffer.mNumber);
        mColorBuffer = std::move(frameBuffer.mColorBuffer);
        mDepthBuffer = std::move(frameBuffer.mDepthBuffer);

        frameBuffer.mId = frameBuffer.mH = frameBuffer.mW = frameBuffer.mNumber = 0;
        frameBuffer.mColorBuffer.destroy();
        frameBuffer.mDepthBuffer.destroy();

        return *this;
    }

    void FrameBuffer::create(void)
    {
        destroy();
        glGenFramebuffers(1, &mId);
    }

    void FrameBuffer::createTexture(u32 w, u32 h,
                                    vector<FormatType> const &internalFormat,
                                    bool depth)
    {
        if(mId == 0)
            throw Except("FrameBuffer is not create");

        mNumber = internalFormat.size();

        mColorBuffer.create(mNumber);

        for(u32 i = 0; i < mNumber; ++i)
        {
            mColorBuffer.emptyTexture(i, w, h, internalFormat[i]);
            glNamedFramebufferTexture2DEXT(mId, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                           mColorBuffer.mId[i], 0);
        }

        if(depth == true)
        {
            mDepthBuffer.create(1);

            mDepthBuffer.emptyDepthTexture(0, w, h);

            glNamedFramebufferTexture2DEXT(mId, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                           mDepthBuffer.mId[0], 0);
        }

        // Les textures ont la même taille
        mW = w;
        mH = h;

        glFramebufferDrawBuffersEXT(mId, mNumber, attachments);
    }

    void FrameBuffer::createTextureArray(u32 number, u32 w, u32 h, const std::vector<FormatType> &internalFormat, bool depth)
    {
        if(mId == 0)
            throw Except("FrameBuffer is not create");

        mNumber = internalFormat.size();

        mColorBuffer.create(mNumber);

        for(u32 i = 0; i < mNumber; ++i)
            mColorBuffer.emptyTextureArray(i, number, w, h, internalFormat[i]);

        if(depth == true)
        {
            mDepthBuffer.create(1);
            mDepthBuffer.emptyDepthTextureArray(0, number, w, h);
        }

        // Les textures ont la même taille
        mW = w;
        mH = h;

        glFramebufferDrawBuffersEXT(mId, mNumber, attachments);
    }

    void FrameBuffer::createCubeMap(u32 w, u32 h, vector<FormatType> const &internalFormat, bool depth)
    {
        if(mId == 0)
            throw Except("FrameBuffer is not create");

        mNumber = internalFormat.size();

        mColorBuffer.create(mNumber);

        for(u32 i = 0; i < mNumber; ++i)
            mColorBuffer.emptyCubeMap(i, w, h, internalFormat[i]);

        if(depth == true)
        {
            mDepthBuffer.create(1);
            mDepthBuffer.emptyDepthCubeMap(0, w, h);
        }

        // Les textures ont la même taille
        mW = w;
        mH = h;

        glFramebufferDrawBuffersEXT(mId, mNumber, attachments);
    }

    void FrameBuffer::createCubeMapArray(u32 number, u32 w, u32 h, const std::vector<FormatType> &internalFormat, bool depth)
    {
        if(mId == 0)
            throw Except("FrameBuffer is not create");

        mNumber = internalFormat.size();

        mColorBuffer.create(mNumber);

        for(u32 i = 0; i < mNumber; ++i)
            mColorBuffer.emptyCubeMapArray(i, number, w, h, internalFormat[i]);

        if(depth == true)
        {
            mDepthBuffer.create(1);
            mDepthBuffer.emptyDepthCubeMapArray(0, number, w, h);
        }

        // Les textures ont la même taille
        mW = w;
        mH = h;

        glFramebufferDrawBuffersEXT(mId, mNumber, attachments);
    }

    void FrameBuffer::attachCubeMap(CubeMap target)
    {
        for(u32 i = 0; i < mNumber; ++i)
            glNamedFramebufferTexture2DEXT(mId, GL_COLOR_ATTACHMENT0 + i, target, mColorBuffer.mId[i], 0);

        if(mDepthBuffer.isCreate())
            glNamedFramebufferTexture2DEXT(mId, GL_DEPTH_ATTACHMENT, target, mDepthBuffer.mId[0], 0);
    }

    void FrameBuffer::attachTextureArray(u32 index)
    {
        for(u32 i = 0; i < mNumber; ++i)
            glNamedFramebufferTextureLayerEXT(mId, GL_COLOR_ATTACHMENT0 + i, mColorBuffer.mId[i], 0, index);

        if(mDepthBuffer.isCreate())
            glNamedFramebufferTextureLayerEXT(mId, GL_DEPTH_ATTACHMENT, mDepthBuffer.mId[0], 0, index);
    }

    void FrameBuffer::attachCubeMapArray(CubeMap target, u32 index)
    {
        u32 indexFace = target - POS_X + index * 6;
        for(u32 i = 0; i < mNumber; ++i)
            glNamedFramebufferTextureLayerEXT(mId, GL_COLOR_ATTACHMENT0 + i, mColorBuffer.mId[i], 0, indexFace);

        if(mDepthBuffer.isCreate())
            glNamedFramebufferTextureLayerEXT(mId, GL_DEPTH_ATTACHMENT, mDepthBuffer.mId[0], 0, indexFace);
    }

    void FrameBuffer::bind(void)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mId);
        glViewport(0, 0, mW, mH);
    }

    void FrameBuffer::bindTextures(u32 indexFirstTexture, u32 firstUnit, u32 count)
    {
        mColorBuffer.bindTextures(indexFirstTexture, firstUnit, count);
    }

    void FrameBuffer::bindImages(u32 indexFirstImage, u32 firstUnit, u32 count)
    {
        mColorBuffer.bindImages(indexFirstImage, firstUnit, count);
    }

    void FrameBuffer::bindDepthBufferTexture(u32 firstUnit)
    {
        mDepthBuffer.bindTextures(0, firstUnit, 1);
    }

    void FrameBuffer::destroy(void)
    {
        if(mId != 0)
        {
            glDeleteFramebuffers(1, &mId);
            mColorBuffer.destroy();
            mDepthBuffer.destroy();
            mId = 0;
            mW = 0;
            mH = 0;
            mNumber = 0;
        }
    }

    FrameBuffer::~FrameBuffer(void)
    {
        destroy();
    }

    void renderIntoCubeMap(const std::shared_ptr<FrameBuffer> &frameBuffer, const vec3 &pos, float far, const std::shared_ptr<Shader> &shader)
    {
        shared_ptr<CameraStatic> camera;

        frameBuffer->bind();

        for(u32 i = 0; i < 6; ++i)
        {
            camera = make_shared<CameraStatic>(pos, pos + look[i], up[i], radians(90.0f), 1.0f, 1.0f, far);

            frameBuffer->attachCubeMap(CubeMap(POS_X + i));
            global->device->clearDepthColorBuffer();
            global->device->clearStencilBuffer();

            global->sceneManager->pushModelsInPipeline(camera);
            global->Shaders.depth->use();
                global->sceneManager->renderDepthPass();

            shader->use();
                global->sceneManager->renderModels();

            synchronize();
        }
    }

    void renderIntoCubeMapArray(shared_ptr<FrameBuffer> const &frameBuffer,
                                u32 index, vec3 const &pos, float far, shared_ptr<Shader> const &shader)
    {
        shared_ptr<CameraStatic> camera;

        frameBuffer->bind();

        for(u32 i = 0; i < 6; ++i)
        {
            camera = make_shared<CameraStatic>(pos, pos + look[i], up[i], radians(90.0f), 1.0f, 1.0f, far);

            frameBuffer->attachCubeMapArray(CubeMap(POS_X + i), index);
            global->device->clearDepthColorBuffer();

            global->sceneManager->pushModelsInPipeline(camera);
            global->Shaders.depth->use();
                global->sceneManager->renderDepthPass();

            shader->use();
                global->sceneManager->renderModels();

            synchronize();
        }
    }
}
