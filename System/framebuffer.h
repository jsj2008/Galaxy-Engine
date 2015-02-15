/*!
 * \file framebuffer.h
 * \brief Create Frame Buffer Object
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../include/include.h"
#include "texture.h"
#include "../Debug/debug.h"

namespace GXY
{
    /**
      * @example FrameBuffer fboExample.cpp
      * @code{.cpp}
      * GXY::FrameBuffer FBO;
      * FBO.create();
      * FBO.createTexture(1024, 1024, {RGBA8_UNORM}, true, false);
      *
      * FBO.bind();
      * drawing();
      *
      * FBO.bindTextures(0, 0, 1);
      * // Use texturing
      * @endcode
      */

    /**
     * @brief The FrameBuffer Object
     *
     * The main objective behind a FrameBuffer is to render on the texture
     * insted of rendering on the screen
     */
    class FrameBuffer
    {
    public:
        /**
         * @brief FrameBuffer Constructor
         */
        FrameBuffer(void);

        /**
         * @brief FrameBuffer Constructor move semantic
         * @param frameBuffer
         */
        FrameBuffer(FrameBuffer &&frameBuffer);
        FrameBuffer(FrameBuffer const &frameBuffer) = delete;

        /**
         * @brief FrameBuffer move semantic
         * @param frameBuffer
         * @return
         */
        FrameBuffer &operator=(FrameBuffer &&frameBuffer);
        FrameBuffer &operator=(FrameBuffer const &frameBuffer) = delete;

        /**
         * @brief Create one and only one FrameBuffer
         */
        void create(void);

        /**
         * @brief Create one or several empty Texture (color, depth, and stencil)
         * @param w : Width of each Texture
         * @param h : Height of each Texture
         * @param internalFormat : vector of FormatType for each Texture
         * @param depth : add a depth Texture (aka depthBuffer)
         * @param stencil : add a stencil Texture (aka stencil Buffer)
         */
        void createTexture(u32 w, u32 h,
                           std::vector<FormatType> const &internalFormat,
                           bool depth, bool stencil);

        /**
         * @brief Create one or several empty Cube Map Textures
         * @param w : Width of each Texture in CubeMap
         * @param h : Height of each Texture in CubeMap
         * @param internalFormat : vector of FormatType each Texture
         * @param depth : Add a depth Texture (only one)
         * @param stencil : Add a stencil Texture (only one)
         */
        void createCubeMapTexture(u32 w, u32 h, std::vector<FormatType> const &internalFormat, bool depth, bool stencil);
        
        /**
         * @brief Let to attach the Good Texture (6 faces) at the FrameBuffer
         * @param target : X Positive to Z Negative
         */
        void attachCubeMapLayer(GLenum target);

        /**
         * @brief Bind the FrameBuffer to draw inside it
         */
        void bind(void);

        /**
         * @brief Bind Textures on FrameBuffer to OpenGL Sampler
         * @param indexFirstTexture : If you don't want to bind all textures
         * @param firstUnit : First Sampler2D / SamplerCube unit binding
         * @param count : Number of textures to bind
         */
        void bindTextures(u32 indexFirstTexture, u32 firstUnit, u32 count);
        
        /**
         * @brief Bind images on FrameBuffer to OpenGL Image
         * @param indexFirstImage : If you don't want to bind all image
         * @param firstUnit : First Sampler2D / SamplerCube unit binding
         * @param count : Number of images to bind
         */
        void bindImages(u32 indexFirstImage, u32 firstUnit, u32 count);
        
        /**
         * @brief Destroy FrameBuffer and Texture
         */
        void destroy(void);

        /**
          * @brief FrameBuffer Destructor
          */
        ~FrameBuffer(void);

    private:
        u32 mId; //!< ID OpenGL for FrameBuffer
        u32 mNumber; //!< Number Textures
        u32 mW, mH; //!< Size of Textures
        Texture mTexture; //!< Texture
    };
}

#endif // FRAMEBUFFER_H
