/*!
 * \file vertexarray.h
 * \brief VertexArray
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "../include/include.h"
#include "../include/constant.h"
#include "../Debug/debug.h"
#include "buffer.h"

namespace GXY
{

    /**
     * @brief Vertex Array Object
     *
     * Provide an interface to rendering with buffer, is like display list in old OpenGL
     */
    class VertexArray
    {
    public :
        /**
         * @brief VertexArray Constructor
         */
        VertexArray(void);

        /**
         * @brief VertexArray Constructor Move semantic
         * @param va
         */
        VertexArray(VertexArray &&va);
        VertexArray(VertexArray const &va) = delete;

        /**
         * @brief VertexArray move semantic
         * @param va
         * @return
         */
        VertexArray &operator=(VertexArray &&va);
        VertexArray &operator=(VertexArray const &va) = delete;

        /**
         * @brief Create one VertexArray
         */
        void create(void);

        /**
         * @brief Bind VertexArray
         */
        void bind(void);

        /**
         * @brief Configure VertexArray for Vertex
         * @param buffer : Buffer<Vertex>
         */
        void configure(Buffer<Vertex> const &buffer);

        /**
         * @brief Configure VertexArray for vec2
         * @param buffer : Buffer<vec2>
         */
        void configure(Buffer<glm::vec2> const &buffer);

        /**
         * @brief Configure VertexArray for vec3
         * @param buffer : Buffer<vec3>
         */
        void configure(Buffer<glm::vec3> const &buffer);

        /**
         * @brief Configure VertexArray for vec4
         * @param buffer : Buffer<vec4>
         */
        void configure(Buffer<glm::vec4> const &buffer);

        /**
         * @brief Bind one Element Buffer to this VertexArray
         * @param buffer
         */
        void bindElementBuffer(Buffer<u32> const &buffer);

        /**
         * @brief Destroy ressources
         */
        void destroy(void);

        /**
          * @brief VertexArray Destructor
          */
        ~VertexArray(void);

    private :
        u32 mId; //!< ID of VertexArray
    };
}

#endif // VERTEXARRAY_H
