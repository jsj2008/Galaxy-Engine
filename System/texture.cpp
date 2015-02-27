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

    void getFilterFromInternal(FormatType internalFormat, GLenum &filter)
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
            filter = GL_LINEAR;
        }

        // Integral don't allow linear filter
        else
        {
           filter = GL_NEAREST;
        }
    }

    void Texture::emptyTexture(u32 index, u32 w, u32 h, FormatType internalFormat)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        GLenum filter;

        getFilterFromInternal(internalFormat, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureStorage2DEXT(mId[index], GL_TEXTURE_2D, 1, internalFormat, w, h);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyTextureArray(u32 index, u32 number, u32 w, u32 h, FormatType internalFormat)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        GLenum filter;

        getFilterFromInternal(internalFormat, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureStorage3DEXT(mId[index], GL_TEXTURE_2D_ARRAY, 1, internalFormat, w, h, number);

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

        glTextureStorage2DEXT(mId[index], GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, w, h);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyDepthTextureArray(u32 index, u32 number, u32 w, u32 h)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureStorage3DEXT(mId[index], GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32, w, h, number);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyCubeMap(u32 index, u32 w, u32 h, FormatType internalFormat)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        GLenum filter;

        getFilterFromInternal(internalFormat, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTextureStorage2DEXT(mId[index], GL_TEXTURE_CUBE_MAP, 1, internalFormat, w, h);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyCubeMapArray(u32 index, u32 number, u32 w, u32 h, FormatType internalFormat)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        GLenum filter;

        getFilterFromInternal(internalFormat, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, filter);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTextureStorage3DEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, 1, internalFormat, w, h, 6 * number);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyDepthCubeMap(u32 index, u32 w, u32 h)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTextureStorage2DEXT(mId[index], GL_TEXTURE_CUBE_MAP, 1, GL_DEPTH_COMPONENT32, w, h);

        mW[index] = w;
        mH[index] = h;
    }

    void Texture::emptyDepthCubeMapArray(u32 index, u32 number, u32 w, u32 h)
    {
        if(index >= mId.size())
            throw Except("Texture : Index out of rang");

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteriEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTextureStorage3DEXT(mId[index], GL_TEXTURE_CUBE_MAP_ARRAY, 1, GL_DEPTH_COMPONENT32, w, h, 6 * number);

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
