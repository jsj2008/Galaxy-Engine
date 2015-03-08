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
         * @param[in] frameBuffer
         */
        FrameBuffer(FrameBuffer &&frameBuffer);
        FrameBuffer(FrameBuffer const &frameBuffer) = delete;

        /**
         * @brief FrameBuffer move semantic
         * @param[in] frameBuffer
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
         * @param[in] w : Width of each Texture
         * @param[in] h : Height of each Texture
         * @param[in] internalFormat : vector of FormatType for each ColorAttachment
         * @param[in] depth : add a depth Texture (aka depthBuffer)
         */
        void createTexture(u32 w, u32 h, std::vector<FormatType> const &internalFormat, bool depth);

        void createTextureArray(u32 number, u32 w, u32 h, std::vector<FormatType> const &internalFormat, bool depth);

        /**
         * @brief Create one or several empty Cube Map Textures
         * @param[in] w : Width of each Texture in CubeMap
         * @param[in] h : Height of each Texture in CubeMap
         * @param[in] internalFormat : vector of FormatType for each ColorAttachment
         * @param[in] depth : Add a depth Texture
         */
        void createCubeMap(u32 w, u32 h, std::vector<FormatType> const &internalFormat, bool depth);

        void createCubeMapArray(u32 number, u32 w, u32 h, std::vector<FormatType> const &internalFormat, bool depth);
        
        /**
         * @brief Let to attach the Good Texture (6 faces) at the FrameBuffer
         * @param[in] target : X Positive to Z Negative
         */
        void attachCubeMap(CubeMap target);

        /**
         * @brief Let to attache the good Texture on the Array at the FrameBuffer
         * @param[in] index : Index of Array
         */
        void attachTextureArray(u32 index);

        /**
         * @brief Fusion of attachCubeMap and attachTextureArray for CubeMapArray
         * @param[in] target : X Positive to Z Negative
         * @param[in] index : Index of Array
         */
        void attachCubeMapArray(CubeMap target, u32 index);

        /**
         * @brief Bind the FrameBuffer to draw inside it
         */
        void bind(void);

        /**
         * @brief Bind Textures on FrameBuffer to OpenGL Sampler
         * @param[in] indexFirstTexture : If you don't want to bind all textures
         * @param[in] firstUnit : First Sampler2D / SamplerCube unit binding
         * @param[in] count : Number of textures to bind
         */
        void bindTextures(u32 indexFirstTexture, u32 firstUnit, u32 count);

        /**
         * @brief Bind Texture DepthBuffer on FrameBuffer to OpenGL Sampler
         * @param[in] firstUnit : first Sampler unit binding
         */
        void bindDepthBufferTexture(u32 firstUnit);
        
        /**
         * @brief Bind images on FrameBuffer to OpenGL Image
         * @param[in] indexFirstImage : If you don't want to bind all image
         * @param[in] firstUnit : First Sampler2D / SamplerCube unit binding
         * @param[in] count : Number of images to bind
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
        u32 mNumber; //!< Number Color Buffer
        u32 mW, mH; //!< Size of Textures
        Texture mColorBuffer, mDepthBuffer; //!< Textures
    };

    class CameraStatic;
    class Shader;

    void renderIntoCubeMap(std::shared_ptr<FrameBuffer> const &frameBuffer,
                           glm::vec3 const &pos, float far, std::shared_ptr<Shader> const &shader);

    void renderIntoCubeMapArray(std::shared_ptr<FrameBuffer> const &frameBuffer,
                                u32 index, glm::vec3 const &pos, float far, std::shared_ptr<Shader> const &shader);
}

#endif // FRAMEBUFFER_H
