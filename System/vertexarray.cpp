/*!
 * \file vertexarray.cpp
 * \brief VertexArray
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "vertexarray.h"

namespace GXY
{
    using namespace std;
    using namespace glm;

    VertexArray::VertexArray(void) : mId(0)
    {

    }

    VertexArray::VertexArray(VertexArray &&va)
    {
        mId = std::move(va.mId);

        va.mId = 0;
    }

    VertexArray &VertexArray::operator=(VertexArray &&va)
    {
        mId = std::move(va.mId);

        va.mId = 0;

        return *this;
    }

    void VertexArray::create(void)
    {
        destroy();
        glGenVertexArrays(1, &mId);
    }

    void VertexArray::bind(void)
    {
        if(mId == 0)
            throw Except("Vertex Array is not initialize");

        glBindVertexArray(mId);
    }

    void VertexArray::configure(const Buffer<vec2> &buffer)
    {
        if(mId == 0)
            throw Except("Vertex Array is not initialize");

        bind();
            glEnableVertexAttribArray(0);
            glVertexAttribBinding(0, 0);
            glVertexAttribFormat(0, 2, GL_FLOAT, false, 0);
            glBindVertexBuffer(0, buffer.mId, 0, sizeof(vec2));
    }

    void VertexArray::configure(const Buffer<vec3> &buffer)
    {
        if(mId == 0)
            throw Except("Vertex Array is not initialize");

        bind();
            glEnableVertexAttribArray(0);
            glVertexAttribBinding(0, 0);
            glVertexAttribFormat(0, 3, GL_FLOAT, false, 0);
            glBindVertexBuffer(0, buffer.mId, 0, sizeof(vec3));
    }

    void VertexArray::configure(const Buffer<vec4> &buffer)
    {
        if(mId == 0)
            throw Except("Vertex Array is not initialize");

        bind();
            glEnableVertexAttribArray(0);
            glVertexAttribBinding(0, 0);
            glVertexAttribFormat(0, 4, GL_FLOAT, false, 0);
            glBindVertexBuffer(0, buffer.mId, 0, sizeof(vec4));
    }

    void VertexArray::configure(const Buffer<Vertex> &buffer)
    {
        if(mId == 0)
            throw Except("Vertex Array is not initialize");

        bind();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);

            glVertexAttribBinding(0, 0);
            glVertexAttribBinding(1, 0);
            glVertexAttribBinding(2, 0);
            glVertexAttribBinding(3, 0);

            glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(Vertex, position));
            glVertexAttribFormat(1, 3, GL_FLOAT, false, offsetof(Vertex, normal));
            glVertexAttribFormat(2, 2, GL_FLOAT, false, offsetof(Vertex, texCoord));
            glVertexAttribIFormat(3, 1, GL_UNSIGNED_INT, offsetof(Vertex, materialIndex));

            glBindVertexBuffer(0, buffer.mId, 0, sizeof(Vertex));
    }

    void VertexArray::bindElementBuffer(const Buffer<u32> &buffer)
    {
        if(mId == 0)
            throw Except("Vertex Array is not initialize");

        bind();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.mId);
    }

    void VertexArray::destroy(void)
    {
        if(mId != 0)
        {
            glDeleteVertexArrays(1, &mId);
            mId = 0;
        }
    }

    VertexArray::~VertexArray(void)
    {
        destroy();
    }
}
