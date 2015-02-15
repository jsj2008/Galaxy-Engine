/*!
 * \file texture.cpp
 * \brief Let to create empty texture or load images
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "texture.h"

namespace GXY
{
    using namespace std;

    Texture::Texture(void)
    {

    }

    Texture::Texture(Texture &&texture)
    {
        mId = std::move(texture.mId);
        mW = std::move(texture.mW);
        mH = std::move(texture.mH);

        texture.mH.clear();
        texture.mW.clear();
        texture.mId.clear();
    }

    Texture &Texture::operator=(Texture &&texture)
    {
        mId = std::move(texture.mId);
        mW = std::move(texture.mW);
        mH = std::move(texture.mH);

        texture.mH.clear();
        texture.mW.clear();
        texture.mId.clear();

        return *this;
    }

    Texture::Texture(u32 number)
    {
        create(number);
    }

    void Texture::create(u32 number)
    {
        destroy();

        mId.resize(number);
        mW.resize(number);
        mH.resize(number);

        glGenTextures(number, &mId[0]);
    }

    u8 *getImage(SDL_Surface *image, u32 &w, u32 &h, GLenum &internalFormat, GLenum &format)
    {
        if(image->format->BytesPerPixel == 3)
        {
            internalFormat = GL_RGB;

            if(image->format->Rmask == 0x000000FF)
                format = GL_RGB;

            else
                format = GL_BGR;
        }

        else
        {
            internalFormat = GL_RGBA;

            if(image->format->Rmask == 0x000000FF)
                format = GL_RGBA;

            else
                format = GL_BGRA;
        }

        w = image->w;
        h = image->h;

        return (u8*)image->pixels;
    }

    void Texture::image(u32 index, string const &path)
    {
        if(index > mId.size())
            throw Except("Texture : Index out of rang");

        u8 *image;
        GLenum format, internalFormat;
        u32 w, h;

        SDL_Surface *img = IMG_Load(path.c_str());

        if(img == nullptr)
            throw Except(string("Impossible to open : ") + path);

        image = getImage(img, w, h, internalFormat, format);

        glTextureImage2DEXT(mId[index], GL_TEXTURE_2D, 0, internalFormat,
                            w, h, 0, format, GL_UNSIGNED_BYTE, image);

        glGenerateTextureMipmapEXT(mId[index], GL_TEXTURE_2D);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        mW[index] = w;
        mH[index] = h;

        SDL_FreeSurface(img);
    }

    void getFormatTypeFilterFromInternal(FormatType internalFormat, GLenum &format, GLenum &type, GLenum &filter)
    {
        /** This part is for each format... **/

        // Float allow linear filter
        if(internalFormat == FormatType::R16F || internalFormat == FormatType::R32F ||
           internalFormat == FormatType::RG16F || internalFormat == FormatType::RG32F ||
           internalFormat == FormatType::RGB16F || internalFormat == FormatType::RGB32F ||
           internalFormat == FormatType::RGBA16F || internalFormat == FormatType::RGBA32F ||

           internalFormat == FormatType::R8_SNORM || internalFormat == FormatType::R16_SNORM ||
           internalFormat == FormatType::RG8_SNORM || internalFormat == FormatType::RG16_SNORM ||
           internalFormat == FormatType::RGB8_SNORM || internalFormat == FormatType::RGB16_SNORM ||
           internalFormat == FormatType::RGBA8_SNORM || internalFormat == FormatType::RGBA16_SNORM ||

           internalFormat == FormatType::R8_UNORM || internalFormat == FormatType::R16_UNORM ||
           internalFormat == FormatType::RG8_UNORM || internalFormat == FormatType::RG16_UNORM ||
           internalFormat == FormatType::RGB8_UNORM || internalFormat == FormatType::RGB16_UNORM ||
           internalFormat == FormatType::RGBA8_UNORM || internalFormat == FormatType::RGBA16_UNORM)
        {
            if(internalFormat == FormatType::R16F || internalFormat == FormatType::R32F)
            {
                format = GL_RED;
                type = GL_FLOAT;
            }

            else if(internalFormat == FormatType::RG16F || internalFormat == FormatType::RG32F)
            {
                format = GL_RG;
                type = GL_FLOAT;
            }

            else if(internalFormat == FormatType::RGB16F || internalFormat == FormatType::RGB32F)
            {
                format = GL_RGB;
                type = GL_FLOAT;
            }

            else if(internalFormat == FormatType::RGBA16F || internalFormat == FormatType::RGBA32F)
            {
                format = GL_RGBA;
                type = GL_FLOAT;
            }

            // byte normalized
            else if(internalFormat == FormatType::R8_SNORM)
            {
                format = GL_RED;
                type = GL_BYTE;
            }

            else if(internalFormat == FormatType::RG8_SNORM)
            {
                format = GL_RG;
                type = GL_BYTE;
            }

            else if(internalFormat == FormatType::RGB8_SNORM)
            {
                format = GL_RGB;
                type = GL_BYTE;
            }

            else if(internalFormat == FormatType::RGBA8_SNORM)
            {
                format = GL_RGBA;
                type = GL_BYTE;
            }

            // short normalized
            else if(internalFormat == FormatType::R16_SNORM)
            {
                format = GL_RED;
                type = GL_SHORT;
            }

            else if(internalFormat == FormatType::RG16_SNORM)
            {
                format = GL_RG;
                type = GL_SHORT;
            }

            else if(internalFormat == FormatType::RGB16_SNORM)
            {
                format = GL_RGB;
                type = GL_SHORT;
            }

            else if(internalFormat == FormatType::RGBA16_SNORM)
            {
                format = GL_RGBA;
                type = GL_SHORT;
            }

            // ubyte normalized
            else if(internalFormat == FormatType::R8_UNORM)
            {
                format = GL_RED;
                type = GL_UNSIGNED_BYTE;
            }

            else if(internalFormat == FormatType::RG8_UNORM)
            {
                format = GL_RG;
                type = GL_UNSIGNED_BYTE;
            }

            else if(internalFormat == FormatType::RGB8_UNORM)
            {
                format = GL_RGB;
                type = GL_UNSIGNED_BYTE;
            }

            else if(internalFormat == FormatType::RGBA8_UNORM)
            {
                format = GL_RGBA;
                type = GL_UNSIGNED_BYTE;
            }

            // ushort unnormalized
            else if(internalFormat == FormatType::R16_UNORM)
            {
                format = GL_RED;
                type = GL_SHORT;
            }

            else if(internalFormat == FormatType::RG16_UNORM)
            {
                format = GL_RG;
                type = GL_UNSIGNED_SHORT;
            }

            else if(internalFormat == FormatType::RGB16_UNORM)
            {
                format = GL_RGB;
                type = GL_UNSIGNED_SHORT;
            }

            else if(internalFormat == FormatType::RGBA16_UNORM)
            {
                format = GL_RGBA;
                type = GL_UNSIGNED_SHORT;
            }

            filter = GL_LINEAR;
        }

        // Integral don't allow linear filter
        else
        {
            // Byte
            if(internalFormat == FormatType::R8I)
            {
                format = GL_RED_INTEGER;
                type = GL_BYTE;
            }

            else if(internalFormat == FormatType::RG8I)
            {
                format = GL_RG_INTEGER;
                type = GL_BYTE;
            }

            else if(internalFormat == FormatType::RGB8I)
            {
                format = GL_RGB_INTEGER;
                type = GL_BYTE;
            }

            else if(internalFormat == FormatType::RGBA8I)
            {
                format = GL_RGBA_INTEGER;
                type = GL_BYTE;
            }

            // Unsigned Byte
            else if(internalFormat == FormatType::R8UI)
            {
                format = GL_RED_INTEGER;
                type = GL_UNSIGNED_BYTE;
            }

            else if(internalFormat == FormatType::RG8UI)
            {
                format = GL_RG_INTEGER;
                type = GL_UNSIGNED_BYTE;
            }

            else if(internalFormat == FormatType::RGB8UI)
            {
                format = GL_RGB_INTEGER;
                type = GL_UNSIGNED_BYTE;
            }

            else if(internalFormat == FormatType::RGBA8UI)
            {
                format = GL_RGBA_INTEGER;
                type = GL_UNSIGNED_BYTE;
            }

            // Short
            else if(internalFormat == FormatType::R16I)
            {
                format = GL_RED_INTEGER;
                type = GL_SHORT;
            }

            else if(internalFormat == FormatType::RG16I)
            {
                format = GL_RG_INTEGER;
                type = GL_SHORT;
            }

            else if(internalFormat == FormatType::RGB16I)
            {
                format = GL_RGB_INTEGER;
                type = GL_SHORT;
            }

            else if(internalFormat == FormatType::RGBA16I)
            {
                format = GL_RGBA_INTEGER;
                type = GL_SHORT;
            }

            // Unsigned short
            else if(internalFormat == FormatType::R16UI)
            {
                format = GL_RED_INTEGER;
                type = GL_UNSIGNED_SHORT;
            }

            else if(internalFormat == FormatType::RG16UI)
            {
                format = GL_RG_INTEGER;
                type = GL_UNSIGNED_SHORT;
            }

            else if(internalFormat == FormatType::RGB16UI)
            {
                format = GL_RGB_INTEGER;
                type = GL_UNSIGNED_SHORT;
            }

            else if(internalFormat == FormatType::RGBA16UI)
            {
                format = GL_RGBA_INTEGER;
                type = GL_UNSIGNED_SHORT;
            }

            // Int
            else if(internalFormat == FormatType::R32I)
            {
                format = GL_RED_INTEGER;
                type = GL_INT;
            }

            else if(internalFormat == FormatType::RG32I)
            {
                format = GL_RG_INTEGER;
                type = GL_INT;
            }

            else if(internalFormat == FormatType::RGB32I)
            {
                format = GL_RGB_INTEGER;
                type = GL_INT;
            }

            else if(internalFormat == FormatType::RGBA32I)
            {
                format = GL_RGBA_INTEGER;
                type = GL_INT;
            }

            // Unsigned short
            else if(internalFormat == FormatType::R32UI)
            {
                format = GL_RED_INTEGER;
                type = GL_UNSIGNED_INT;
            }

            else if(internalFormat == FormatType::RG32UI)
            {
                format = GL_RG_INTEGER;
                type = GL_UNSIGNED_INT;
            }

            else if(internalFormat == FormatType::RGB32UI)
            {
                format = GL_RGB_INTEGER;
                type = GL_UNSIGNED_INT;
            }

            else if(internalFormat == FormatType::RGBA32UI)
            {
                format = GL_RGBA_INTEGER;
                type = GL_UNSIGNED_INT;
            }

            else if(internalFormat == FormatType::RGB10_A2UI)
            {
                format = GL_RGBA_INTEGER;
                type = GL_UNSIGNED_INT_10_10_10_2;
            }

           filter = GL_NEAREST;
        }
    }

    void Texture::emptyTexture(u32 index, u32 w, u32 h, FormatType internalFormat)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        GLenum format, type, filter;

        getFormatTypeFilterFromInternal(internalFormat, format, type, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureImage2DEXT(mId[index], GL_TEXTURE_2D, 0, (u32)internalFormat, w, h, 0,
                            format, type, nullptr);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyCubeMap(u32 index, u32 w, u32 h, FormatType internalFormat)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        GLenum format, type, filter;

        getFormatTypeFilterFromInternal(internalFormat, format, type, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for(u32 i = 0; i < 6; ++i)
            glTextureImage2DEXT(mId[index], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (u32)internalFormat, w, h, 0,
                                format, type, nullptr);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyDepthTexture(u32 index, u32 w, u32 h)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureImage2DEXT(mId[index], GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
                            w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyStencilTexture(u32 index, u32 w, u32 h)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureImage2DEXT(mId[index], GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8,
                            w, h, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::bindTextures(u32 indexFirstTexture, u32 firstUnit, u32 count) const
    {
        if(indexFirstTexture >= mId.size() || count + indexFirstTexture > mId.size())
            throw Except("Texture : Index out of rang");

        glBindTextures(firstUnit, count, &mId[indexFirstTexture]);
    }

    void Texture::bindImages(u32 indexFirstImage, u32 firstUnit, u32 count) const
    {
        if(indexFirstImage >= mId.size() || count + indexFirstImage > mId.size())
            throw Except("Texture : Index out of rang");

        glBindImageTextures(firstUnit, count, &mId[indexFirstImage]);
    }

    u64 Texture::getHandle(u32 index)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        u64 handle = glGetTextureHandleARB(mId[index]);
        glMakeTextureHandleResidentARB(handle);
        return handle;
    }

    void Texture::destroy(void)
    {
        if(mId.size() != 0)
        {
            glDeleteTextures(mId.size(), &mId[0]);
            mId.clear();
            mW.clear();
            mH.clear();
        }
    }

    Texture::~Texture(void)
    {
        destroy();
    }
}
