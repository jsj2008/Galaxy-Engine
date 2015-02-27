/*!
 * \file texture.h
 * \brief Let to create empty texture or load images
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../include/include.h"
#include "../include/constant.h"
#include "../Debug/debug.h"

namespace GXY
{
    class FrameBuffer;

    /**
      * @example Texture texExample.cpp
      * @code{.cpp}
      * GXY::Texture tex;
      *
      * tex.create(1);
      * tex.image(0, "image.png");
      *
      * vao.bind();
      * shader.use();
      * tex.bindTextures(0, 0, 1);
      *
      * draw...
      * @endcode
      */

    /**
     * @brief Provide an array of Texture, they can be empty or loaded from images
     */
    class Texture
    {
        friend FrameBuffer;
    public:
        /**
         * @brief Texture Constructor
         */
        Texture(void);

        /**
         * @brief Texture Constructor move semantic
         * @param texture
         */
        Texture(Texture &&texture);

        /**
         * @brief Texture move semantic
         * @param texture
         * @return
         */
        Texture &operator=(Texture &&texture);

        Texture(Texture const &texture) = delete;
        Texture &operator=(Texture const &texture) = delete;

        /**
         * @brief Texture Constructor with number of Texture
         * @param number : Number of Texture
         */
        Texture(u32 number);

        /**
         * @brief Create number Texture
         * @param number : Number of Texture
         */
        void create(u32 number);

        bool isCreate(void)
        {
            return mId.size() != 0;
        }

        /**
         * @brief Create one empty texture 2D
         * @param index : Index of this texture
         * @param w : Width of this texture
         * @param h : Height of this texture
         * @param internalFormat : FormatType of this Texture
         */
        void emptyTexture(u32 index, u32 w, u32 h, FormatType internalFormat);

        /**
         * @brief Create one empty CubeMap
         * @param index : Index of this CubeMap
         * @param w : Width of each side of this CubeMap
         * @param h : Height of each side of this CubeMap
         * @param internalFormat : FormatType of each size of this CubeMap
         */
        void emptyCubeMap(u32 index, u32 w, u32 h, FormatType internalFormat);

        /**
         * @brief Create one stencil Texture
         * @param index : Index of this Texture
         * @param w : Width of this Texture
         * @param h : Height of this Texture
         */
        void emptyStencilTexture(u32 index, u32 w, u32 h);

        /**
         * @brief Create one depth Texture
         * @param index : Index of this Texture
         * @param w : Width of this Texture
         * @param h : Height of this Texture
         */
        void emptyDepthTexture(u32 index, u32 w, u32 h);

        /**
         * @brief Load image (path)
         * @param index : Index of this Texture
         * @param path : Path to image
         */
        void image(u32 index, std::string const &path);

        /**
         * @brief Bind images to OpenGL Image
         * @param indexFirstImage : If you don't want to bind all image
         * @param firstUnit : First Sampler2D / SamplerCube unit binding
         * @param count : Number of images to bind
         */
        void bindImages(u32 indexFirstImage, u32 firstUnit, u32 count) const;

        /**
         * @brief Bind Textures on FrameBuffer to OpenGL Sampler
         * @param indexFirstTexture : If you don't want to bind all textures
         * @param firstUnit : First Sampler2D / SamplerCube unit binding
         * @param count : Number of textures to bind
         */
        void bindTextures(u32 indexFirstTexture, u32 firstUnit, u32 count) const;

        /**
         * @brief Get Width of this Texture
         * @param index : Index of this Texture
         * @return Width
         */
        inline u32 getWidth(u32 index) {return mW[index];}

        /**
         * @brief Get Height of this Texture
         * @param index : Index of this Texture
         * @return Height
         */
        inline u32 getHeight(u32 index){return mH[index];}

        /**
         * @brief Get Handle and makeResident this Texture
         * @param index : Index of Texture
         * @return
         */
        u64 getHandle(u32 index);

        /**
         * @brief destroy all ressources
         */
        void destroy(void);

        /**
          * @brief Texture Destructor
          */
        ~Texture(void);

    private:
        std::vector<u32> mId; //!< IDs of texture OpenGL

        std::vector<u32> mW; //!< Widths
        std::vector<u32> mH; //!< Heights
    };
}
#endif // TEXTURE_H
