/*!
 * \file constant.h
 * \brief forgive constants
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef CONSTANT_H
#define CONSTANT_H

#include "include.h"

namespace GXY
{
    /**
      * @brief Describe One Cube of 2 units per side, center in 0, 0, 0
     */
    glm::vec3 const CubeAABB3D[] = {glm::vec3(-1.0f, -1.0f, 1.0),
                                    glm::vec3(1.0f, -1.0f, 1.0f),
                                    glm::vec3(1.0f, 1.0f, 1.0f),
                                    glm::vec3(-1.0f, 1.0f, 1.0f),
                                    glm::vec3(-1.0f, -1.0f, -1.0f),
                                    glm::vec3(1.0f, -1.0f, -1.0f),
                                    glm::vec3(1.0f, 1.0f, -1.0f),
                                    glm::vec3(-1.0f, 1.0f, -1.0f)
                                   };

    /**
      * @brief Forgive index for drawing the prior cube
      */
    u32 const ElementCubeAABB3D[] = {0, 1, 2, 2, 3, 0,
                                     3, 2, 6, 6, 7, 3,
                                     7, 6, 5, 5, 4, 7,
                                     4, 0, 3, 3, 7, 4,
                                     0, 1, 5, 5, 4, 0,
                                     1, 5, 6, 6, 2, 1
                                    };

    /**
      * @brief Like cube, but for drawing a quad
      */
    glm::vec2 const QuadAABB2D[] = {glm::vec2(-1.0, -1.0),
                                    glm::vec2(-1.0,  1.0),
                                    glm::vec2( 1.0, -1.0),
                                    glm::vec2( 1.0,  1.0)
                                   };

    /**
     * @brief Forgive some constants for Buffer
     */
    enum BufferType{ELEMENT = GL_ELEMENT_ARRAY_BUFFER, //!< Element Buffer or Index Buffer
                    DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER, //!< glDraw*Indirect
                    DISPATCH_INDIRECT = GL_DISPATCH_INDIRECT_BUFFER, //!< glComputeDispatchIndirect
                    UNIFORM = GL_UNIFORM_BUFFER, //!< Uniform Buffer : Generally in L1 cache
                    SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER //!< Shader Storage Buffer : Global Memory
                   };

    enum CubeMap{POS_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X, //!< Right Side
                 NEG_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X, //!< Left Side
                 POS_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y, //!< Up side
                 NEG_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, //!< Down Side
                 POS_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z, //!< Backward Side
                 NEG_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z  //!< Forward Side
                };

    /**
     * @brief Forgive some constants for Shader
     */
    enum ShaderType{VERTEX = GL_VERTEX_SHADER, //!< Vertex Shader
                    FRAGMENT = GL_FRAGMENT_SHADER, //!< Fragment (or Pixel) Shader
                    GEOMETRY = GL_GEOMETRY_SHADER, //!< Geometry Shader
                    COMPUTE = GL_COMPUTE_SHADER //!< Compute Shader
                   };

    /**
     * @brief Forgive some constants for AbstractCamera
     */
    enum CameraUp{CAM_UP_X, //!< 1, 0, 0
                  CAM_DOWN_X,
                  CAM_UP_Y, //!< 0, 1, 0
                  CAM_DOWN_Y,
                  CAM_UP_Z, //!< 0, 0, 1
                  CAM_DOWN_Z
                 };

    /**
     * @brief Forgive some constants for Key Mapping in CameraFPS
     */
    enum CameraKeyMap{CAM_FORWARD, //!< Forward Movement
                      CAM_BACKWARD, //!< Backward Movement
                      CAM_LEFT, //!< Left Movement
                      CAM_RIGHT, //!< Right Movement
                      CAM_JUMP, //!< Jump Movement
                      CAM_BOOST //!< Running : speed
                     };

    /**
     * @brief Forgive some constants for Texture
     */
    enum FormatType{R16F = GL_R16F, //!< f16
                    RG16F = GL_RG16F, //!< f16 f16
                    RGB16F = GL_RGB16F, //!< f16 f16 f16
                    RGBA16F = GL_RGBA16F, //!< f16 f16 f16 f16

                    R32F = GL_R32F, //!< f32
                    RG32F = GL_RG32F, //!< f32 f32
                    RGB32F = GL_RGB32F, //!< f32 f32 f32
                    RGBA32F = GL_RGBA32F, //!< f32 f32 f32 f32

                    R8I = GL_R8I, //!< i8
                    RG8I = GL_RG8I, //!< i8 i8
                    RGB8I = GL_RGB8I, //!< i8 i8 i8
                    RGBA8I = GL_RGBA8I, //!< i8 i8 i8 i8

                    R8UI = GL_R8UI, //!< ui8
                    RG8UI = GL_RG8UI, //!< ui8 ui8
                    RGB8UI = GL_RGB8UI, //!< ui8 ui8 ui8
                    RGBA8UI = GL_RGBA8UI, //!< ui8 ui8 ui8 ui8

                    R16I = GL_R16I, //!< i16
                    RG16I = GL_RG16I, //!< i16 i16
                    RGB16I = GL_RGB16I, //!< i16 i16 i16
                    RGBA16I = GL_RGBA16I, //!< i16 i16 i16 i16

                    R16UI = GL_R16UI, //!< ui16
                    RG16UI = GL_RG16UI, //!< ui16 ui16
                    RGB16UI = GL_RGBA16I, //!< ui16 ui16 ui16
                    RGBA16UI = GL_RGBA16UI, //!< ui16 ui16 ui16 ui16

                    R32I = GL_R32I, //!< i32
                    RG32I = GL_RG32I, //!< i32 i32
                    RGB32I = GL_RGB32I, //!< i32 i32 i32
                    RGBA32I = GL_RGBA32I, //!< i32 i32 i32 i32

                    R32UI = GL_R32UI, //!< ui32
                    RG32UI = GL_RG32UI, //!< ui32 ui32
                    RGB32UI = GL_RGB32UI, //!< ui32 ui32 ui32
                    RGBA32UI = GL_RGBA32UI, //!< ui32 ui32 ui32 ui32

                    RGB10_A2UI = GL_RGB10_A2UI, //!< ui10 ui10 ui10 ui2

                    R8_SNORM = GL_R8_SNORM, //!< is8
                    RG8_SNORM = GL_RG8_SNORM, //!< is8 is8
                    RGB8_SNORM = GL_RGB8_SNORM, //!< is8 is8 is8
                    RGBA8_SNORM = GL_RGBA8_SNORM, //!< is8 is8 is8 is8

                    R16_SNORM = GL_R16_SNORM, //!< is16
                    RG16_SNORM = GL_RG16_SNORM, //!< is16 is16
                    RGB16_SNORM = GL_RGB16_SNORM, //!< is16 is16 is16
                    RGBA16_SNORM = GL_RGBA16_SNORM, //!< is16 is16 is16 is16

                    R8_UNORM = GL_R8, //!< us8
                    RG8_UNORM = GL_RG8, //!< us8 us8
                    RGB8_UNORM = GL_RGB8, //!< us8 us8 us8
                    RGBA8_UNORM = GL_RGBA8, //!< us8 us8 us8 us8

                    R16_UNORM = GL_R16, //!< us16
                    RG16_UNORM = GL_RG16, //!< us16 us16
                    RGB16_UNORM = GL_RGB16, //!< us16 us16 us16
                    RGBA16_UNORM = GL_RGBA16, //!< us16 us16 us16 us16
                   };
}

#endif // CONSTANT_H
