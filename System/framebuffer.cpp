/*!
 * \file framebuffer.cpp
 * \brief Create Frame Buffer Object
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "framebuffer.h"
#include "device.h"

namespace GXY
{
    using namespace std;

    FrameBuffer::FrameBuffer(void) : mId(0), mNumber(0), mW(0), mH(0)
    {

    }

    FrameBuffer::FrameBuffer(FrameBuffer &&frameBuffer)
    {
        mId = std::move(frameBuffer.mId);
        mH = std::move(frameBuffer.mH);
        mW = std::move(frameBuffer.mW);
        mNumber = std::move(frameBuffer.mNumber);
        mTexture = std::move(frameBuffer.mTexture);

        frameBuffer.mId = frameBuffer.mH = frameBuffer.mW = frameBuffer.mNumber = 0;
        frameBuffer.mTexture.destroy();
    }

    FrameBuffer &FrameBuffer::operator=(FrameBuffer &&frameBuffer)
    {
        mId = std::move(frameBuffer.mId);
        mH = std::move(frameBuffer.mH);
        mW = std::move(frameBuffer.mW);
        mNumber = std::move(frameBuffer.mNumber);
        mTexture = std::move(frameBuffer.mTexture);

        frameBuffer.mId = frameBuffer.mH = frameBuffer.mW = frameBuffer.mNumber = 0;
        frameBuffer.mTexture.destroy();

        return *this;
    }

    void FrameBuffer::create(void)
    {
        destroy();
        glGenFramebuffers(1, &mId);
    }

    void FrameBuffer::createTexture(u32 w, u32 h,
                                    vector<FormatType> const &internalFormat,
                                    bool depth, bool stencil)
    {
        if(mId == 0)
            throw Except("FrameBuffer is not create");

        static GLenum buf[] = {GL_COLOR_ATTACHMENT0,
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

        mNumber = internalFormat.size();

        if(depth == true)
            ++mNumber;

        if(stencil == true)
            ++mNumber;

        mTexture.create(mNumber);

        for(u32 i = 0; i < internalFormat.size(); ++i)
        {
            mTexture.emptyTexture(i, w, h, internalFormat[i]);
            glNamedFramebufferTexture2DEXT(mId, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                           mTexture.mId[i], 0);
        }

        if(depth == true)
        {
            mTexture.emptyDepthTexture(internalFormat.size(), w, h);

            glNamedFramebufferTexture2DEXT(mId, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                           mTexture.mId[internalFormat.size()], 0);
        }

        if(stencil == true)
        {
            mTexture.emptyStencilTexture(internalFormat.size(), w, h);
            glNamedFramebufferTexture2DEXT(mId, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                                           mTexture.mId[internalFormat.size()], 0);
        }

        glFramebufferDrawBuffersEXT(mId, internalFormat.size(), buf);

        // Les textures ont la même taille
        mW = w;
        mH = h;
    }

    void FrameBuffer::createCubeMapTexture(u32 w, u32 h,
                                           vector<FormatType> const &internalFormat,
                                           bool depth, bool stencil)
    {
        if(mId == 0)
            throw Except("FrameBuffer is not create");

        mNumber = internalFormat.size();

        if(depth == true)
            ++mNumber;

        if(stencil == true)
            ++mNumber;

        mTexture.create(mNumber);

        for(u32 i = 0; i < internalFormat.size(); ++i)
            mTexture.emptyCubeMap(i, w, h, internalFormat[i]);

        if(depth == true)
        {
            mTexture.emptyDepthTexture(internalFormat.size(), w, h);

            glNamedFramebufferTexture2DEXT(mId, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                           mTexture.mId[internalFormat.size()], 0);
        }

        if(stencil == true)
        {
            mTexture.emptyStencilTexture(internalFormat.size(), w, h);
            glNamedFramebufferTexture2DEXT(mId, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                                           mTexture.mId[internalFormat.size()], 0);
        }

        // Les textures ont la même taille
        mW = w;
        mH = h;
    }

    void FrameBuffer::attachCubeMapLayer(GLenum target)
    {
        static GLenum buf[] = {GL_COLOR_ATTACHMENT0,
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

        for(u32 i = 0; i < mNumber; ++i)
            glNamedFramebufferTexture2DEXT(mId, GL_COLOR_ATTACHMENT0 + i, target, mTexture.mId[i], 0);

        glFramebufferDrawBuffersEXT(mId, mNumber, buf);
    }

    void FrameBuffer::bind(void)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mId);
        glViewport(0, 0, mW, mH);
    }

    void FrameBuffer::bindTextures(u32 indexFirstTexture, u32 firstUnit, u32 count)
    {
        mTexture.bindTextures(indexFirstTexture, firstUnit, count);
    }

    void FrameBuffer::bindImages(u32 indexFirstImage, u32 firstUnit, u32 count)
    {
        mTexture.bindImages(indexFirstImage, firstUnit, count);
    }

    void FrameBuffer::destroy(void)
    {
        if(mId != 0)
        {
            glDeleteFramebuffers(1, &mId);
            mTexture.destroy();
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
}
