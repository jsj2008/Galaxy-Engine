/*!
 * \file buffer.h
 * \brief Forgive a template class Buffer
 * \author Antoine MORRIER
 * \version 1.0
 */

/**
 * @example BufferAndVertexArray bufferExample.cpp
 * @code{.cpp}
 * GXY::Buffer<glm::vec2> buffer;
 * GXY::Buffer<u32> indexBuffer;
 * GXY::VertexArray vertexArray;
 *
 * buffer.create();
 * indexBuffer.create();
 * vertexArray.create();
 *
 * buffer.allocate(3);
 * indexBuffer.allocate(3);
 *
 * buffer[0] = glm::vec2(-1.0, -1.0);
 * buffer[1] = glm::vec2(1.0, -1.0);
 * buffer[2] = glm::vec2(0.0, 1.0);
 * indexBuffer.push(0);
 * indexBuffer.push(1);
 * indexBuffer.push(2);
 *
 * vertexArray.configure(buffer);
 * vertexArray.bindElementBuffer(indexBuffer);
 *
 * vertexArray.bind();
 * glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
 * GXY::synchronize(); // Flush
 * @endcode
 */

#ifndef BUFFER_H
#define BUFFER_H

#include "../include/include.h"
#include "../include/constant.h"

namespace GXY
{
    /**
     * @brief synchronize
     *
     * Wait for the completion for rendering or dispatch command
     */
    inline void synchronize(void)
    {
        GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
        glDeleteSync(fence);
    }

    class VertexArray;

    template<typename T>
    /**
     * @brief The Buffer Object
     *
     * Provide a memory allocation in Video Memory.
     */
    class Buffer
    {
        /*
        * Friend Class
        *  It's to improve encapsulation concept */
        friend VertexArray;

    public:
        /**
         * @brief Buffer Constructor
         */
        Buffer(void) : mId(0), mPtr(nullptr), mNumElementsMax(0), mNumElements(0){}

        /**
         * @brief Buffer Constructor move semantic
         * @param buffer
         */
        Buffer(Buffer<T> &&buffer)
        {
            mId = std::move(buffer.mId);
            mPtr = std::move(buffer.mPtr);
            mNumElementsMax = std::move(buffer.mNumElementsMax);
            mNumElements = std::move(buffer.mNumElements);

            buffer.mId = buffer.mNumElementsMax = buffer.mNumElements = 0;
            buffer.mPtr = nullptr;
        }

        /**
         * @brief Buffer move semantic
         * @param buffer
         * @return newBuffer
         */
        inline Buffer<T> &operator=(Buffer<T> &&buffer)
        {
            mId = std::move(buffer.mId);
            mPtr = std::move(buffer.mPtr);
            mNumElementsMax = std::move(buffer.mNumElementsMax);
            mNumElements = std::move(buffer.mNumElements);

            buffer.mId = buffer.mNumElementsMax = buffer.mNumElements = 0;
            buffer.mPtr = nullptr;

            return *this;
        }

        Buffer(Buffer<T> const &buffer) = delete;
        Buffer<T> &operator=(Buffer<T> const &buffer) = delete;

        /**
         * @brief Allocate nElements in video Memory
         * @param nElements : Number of elements
         */
        inline void allocate(size_t nElements)
        {
            destroy();
            glGenBuffers(1, &mId);

            u32 flags = GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT;

            glNamedBufferStorageEXT(mId, nElements * sizeof(T), nullptr, flags);
            mPtr = (T*)glMapNamedBufferRangeEXT(mId, 0, nElements * sizeof(T), flags);
            mNumElementsMax = nElements;
        }

        /**
         * @brief Multiply by 2 the size maximal of this buffer
         */
        inline void reallocate(void)
        {
            u32 flags = GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT;
            u32 newBuffer;

            if(mNumElementsMax == 0)
                allocate(1);

            // Allocate a new Buffer, copy actually in new buffer, delete actualy
            else
            {
                mNumElementsMax *= 2;

                glGenBuffers(1, &newBuffer);
                glNamedBufferStorageEXT(newBuffer, mNumElementsMax * sizeof(T), nullptr, flags);
                mPtr = (T*)glMapNamedBufferRangeEXT(newBuffer, 0, mNumElementsMax * sizeof(T), flags);
                glNamedCopyBufferSubDataEXT(mId, newBuffer, 0, 0, (mNumElementsMax / 2) * sizeof(T));
                glDeleteBuffers(1, &mId);
                mId = newBuffer;
            }
        }

        /**
         * @brief Copy one Buffer in another one
         * @param buffer : Source Buffer
         * @param readOffset : offset in Source Buffer
         * @param writeOffset : offset in this Buffer
         * @param nElements : number of elements to copy
         */
        inline void copy(Buffer<T> const &buffer, u32 readOffset, u32 writeOffset, size_t nElements)
        {
            glNamedCopyBufferSubDataEXT(buffer.mId, mId, readOffset, writeOffset, nElements * sizeof(T));
        }

        /**
         * @brief Bind the Buffer for rendering command
         * @param type : BufferType of this Buffer : ELEMENT DRAW_INDIRECT DISPATCH_INDIRECT
         */
        inline void bind(BufferType type)
        {
            glBindBuffer(type, mId);
        }

        /**
         * @brief Bind the Buffer to one indexed Buffer target
         * @param type : BufferType of this Buffer : UNIFORM SHADER_STORAGE
         * @param binding : index of this Buffer (binding point)
         */
        inline void bindBase(BufferType type, u32 binding)
        {
            glBindBufferBase(type, binding, mId);
        }

        /**
         * @brief Push one element in the buffer
         * @param[in] elem : Elem which will add on the buffer
         * @param[out] isReallocate : If is reallocate : put it to true, else don't change
         */
        inline void push(T const &elem, bool &isReallocate)
        {
            if(mNumElements == mNumElementsMax)
            {
                isReallocate = true;
                reallocate();
            }

            mPtr[mNumElements++] = elem;
        }

        /**
         * @brief Reset the counter of numElements, so next push is like the first one
         */
        inline void setToZeroElement(void)
        {
            mNumElements = 0;
        }

        /**
         * @brief operator [] like array
         * @param index
         * @return reference on this element
         */
        inline T &operator[](u32 index)
        {
            assert(index < mNumElementsMax);
            mPtr[index];
        }

        /** Bind the buffer range
         * @brief Bind the Buffer to one indexed Buffer target
         * @param type : BufferType of this Buffer : UNIFORM SHADER_STORAGE
         * @param binding : index of this Buffer (binding point)
         * @param offset : offset in number of elements in Buffer
         * @param nElements : number of elements
        */
        inline void bindRange(BufferType type, u32 binding, u32 offset, u32 nElements)
        {
            glBindBufferRange(type, binding, mId, offset * sizeof(T), nElements * sizeof(T));
        }

        /**
         * @brief Get a pointer in "unified memory"
         * @return Pointer of start of Buffer
         */
        inline T *map(void) {return mPtr;}

        /**
         * @brief Get a maximal size of Buffer in number of Elements
         * @return maximal number elements
         */
        inline size_t numMaxElements(void) const {return mNumElementsMax;}

        /**
         * @brief Get the number Elements currently in the buffer
         * @return the number of elements pushed in the buffer
         */
        inline size_t numElements(void) const {return mNumElements;}

        /**
         * @brief Release all ressources
         */
        inline void destroy(void)
        {
            if(mId != 0)
            {
                glDeleteBuffers(1, &mId);
                mId = 0;
                mPtr = nullptr;
                mNumElementsMax = 0;
                mNumElements = 0;
            }
        }

        /**
         * @brief : Destructor of Buffer
         */
        ~Buffer(void)
        {
            destroy();
        }

    private:
        u32 mId; //!< ID of glBuffer
        T* mPtr; //!< Pointer of start of Unified Memory for this Buffer
        size_t mNumElementsMax; //!< Maximal Size for this Buffer
        size_t mNumElements; //!< Number of elements pushed in this Buffer
    };
}

#endif // BUFFER_H
