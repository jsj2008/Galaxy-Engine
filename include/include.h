/*!
 * \file include.h
 * \brief Include all libs and declarations
 * \author Antoine MORRIER
 * \version 1.0
 */

/**
  * @mainpage Galaxy Engine Documentation
  * @section Introduction
  * It's a Graphic Engine Created by Antoine MORRIER, student
  * in Télécom SudParis.
  *
  * This Engine will be able to render global illumination
  * on real time using OpenGL 4.4+ currently.
  *
  * @section Features
  * @subsection Sceneries
  *     This Engine works with Nodes, so all members depend on
  * one other member, for example, glasses in a storage cupboard
  * depend of this storage cupboard.
  *
  * @subsection Ambient Occlusion
  * Ambient Occlusion is in development
  *
  * @subsection Lighting
  * Direct Lighting is in development
  *
  * @subsection Shadows
  * Shadows are in develoment
  *
  * @subsection Global Illumination
  * Global Illumination is in development
  *
  * @subsection EnvironmentMap
  * Reflections, Refractions, Caustics are in development
  */

#ifndef INCLUDE_H
#define INCLUDE_H

// Flux and string
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

//algorithm
#include <algorithm>
#include <functional>

// container
#include <map>
#include <vector>
#include <list>

// Memory
#include <memory>
#include <thread>
#include <cstddef>

// Exception
#include <exception>
#include <stdexcept>

// OpenGL et GLew
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Maths
#define GLM_FORCE_INLINE
#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <cfloat>

// Assimp : Load model
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GXY
{
    typedef GLbyte s8;
    typedef GLshort s16;
    typedef GLint s32;
    typedef GLint64 s64;

    typedef GLubyte u8;
    typedef GLushort u16;
    typedef GLuint u32;
    typedef GLuint64 u64;

    /**
     * \brief It is a struct for glDrawElementsIndirect
     */
    struct DrawElementCommand
    {
        u32 count; //!< Number of index taken on the IndexBuffer
        u32 primCount; //!< Number of instance : 1 in our engine
        u32 firstIndex; //!< first Index if big IndexBuffer
        u32 baseVertex; //!< first Vertex if big VertexBuffer
        u32 baseInstance; //!< first Instance if several Instances : 0 in our engine
    };

    /**
     * @brief It is a struct for glDrawArraysIndirect
     */
    struct DrawArrayCommand
    {
        u32 count; //!< Number of vertex taken on the VertexBuffer
        u32 instanceCount; //!< Number of instance
        u32 first; //!< first vertex taken on the VertexBuffer
        u32 baseInstance; //!< first Instance if several Instances
    };

    /**
     * @brief It is a struct for glDispatchComputeIndirect
     */
    struct DispatchCommand
    {
        u32 num_groups_x; //!< Number of local group in x
        u32 num_groups_y; //!< Number of local group in y
        u32 num_groups_z; //!< Number of local group in z
    };

    /**
     * @brief It is a struct for rendering
     */
    struct Vertex
    {
        glm::vec3 position; //!< Position for vertex
        glm::vec3 normal; //!< Normal for Vertex
        glm::vec3 tangent;
        glm::vec3 biTangent;
        glm::vec2 texCoord; //!< Texture Coordinate for this Vertex
        u32 materialIndex; //!< Index of material for this Vertex
    };

    /**
     * @brief Describe a material of one mesh
     */
    struct Material
    {
        glm::vec4 colorDiffuse; //!< Color of Material if not use Texture
        glm::vec4 shininessAlbedo; //!< Shininess : .x, Albedo : .y
        glm::uvec4 textureHandleUseTexture; //!< TextureHandle : .xy, UseTexture : .z
    };

    /**
     * @brief Describe a bounding box of one mesh
     */
    struct AABB3D
    {
        glm::vec4 coord[8]; //!< One box own 8 vertex
    };

    /**
     * @brief Describe a plane by its equation
     */
    struct Plane
    {
        glm::vec4 plane; //!< Equation of this plane
    };

    /**
     * @brief Create one plane from three points
     * @param[out] plane : plane created
     * @param[in] p0 : first Point
     * @param[in] p1 : second Point
     * @param[in] p2 : third Point
     */
    inline void setPlane(Plane &plane, glm::vec3 const &p0, glm::vec3 const &p1, glm::vec3 const &p2)
    {
        glm::vec3 v = p1 - p0;
        glm::vec3 u = p2 - p0;
        glm::vec3 n = glm::normalize(glm::cross(v, u));
        plane.plane = glm::vec4(n, -glm::dot(p0, n));
    }

    /**
     * @brief Return a distance between the plane and the point, it can be negative
     * @param plane
     * @param p
     * @return
     */
    inline float distancePlane(Plane const &plane, glm::vec3 const &p)
    {
        return glm::dot(plane.plane, glm::vec4(p, 1.0f));
    }

    /**
     * @brief Transform the pos on the AABB3D on the new space describe by transform
     * @param box : A AABB3D "basic"
     * @param transform : The landmark of your future AABB3D
     * @return A new
     */
    inline AABB3D computeAABB3D(AABB3D const &box, glm::mat4 const &transform)
    {
        AABB3D newBox;
        for(u32 i = 0; i < 8; ++i)
            newBox.coord[i] = transform * box.coord[i];
        return newBox;
    }

    /**
     * @brief Describe a sphere by its radius and its position
     */
    struct Sphere
    {
        float radius; //!< Radius of this sphere
        glm::vec3 position; //!< Position of this sphere
    };

    /**
     * \brief Describe some informations to give at Shader
     */
    struct Context
    {
        glm::vec4 inverseSizeFrameBufferAO; //!< .xy = 1 / sizeScreen, .zw = 1 / sizeAO;
    };

    struct FrustrumUniform
    {
        glm::mat4 frustrumMatrix; //!< Is the projectionMatrix product viewMatrix
        glm::vec4 posCamera; //!< .xyz = posCamera or PosLight for shadowMaps for example
        glm::vec4 planesFrustrum[6]; //!< A frustrum is formed by 6 planes
        glm::uvec4 numberMeshesPointLights; //!< NumberMeshed : .x, numberPointLights.y
    };

    /**
     * @brief Describe some informations to give at Shader for PointLight lighting
     */
    struct PointLight
    {
        glm::vec4 positionRadius; //!< .xyz = position, w = radius
        glm::vec4 colorIntensity; //!< .rgb = color, a = intensity
        glm::ivec4 shadowInformation; //!< .x = index of Cube Shadow Map
    };

    /**
     * @brief Describe some informations to give at Shader for Indirect PointLight Lighting
     */
    struct PointLightVPL
    {
        glm::vec4 positionRadius; //*< .xyz = position, w = radius
        glm::vec4 colorIntensity; //*< .rgb = color, a = intensity
        glm::vec4 normal; //*< .xyz : Normal oriented Hemisphere for diffuse indirect lighting
    };
}

#endif // INCLUDE_H
