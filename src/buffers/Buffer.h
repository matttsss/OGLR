#pragma once

#include <GL/glew.h>
#include <iostream>

namespace OGLR::Buffers {

    /// Indicates the type of the buffer
    enum BufferType {
        Index = GL_ELEMENT_ARRAY_BUFFER, Vertex = GL_ARRAY_BUFFER, Uniform = GL_UNIFORM_BUFFER
    };

    /// Indicates how the buffer will be used
    enum UsageType {
        Static = GL_STATIC_DRAW, Dynamic = GL_DYNAMIC_DRAW
    };

    /**
     * Templated buffer class, it does not old data in main memory space
     * @tparam BT (BufferType) Type of buffer
     * @tparam UT (UsageType) Indicates how it will be used
     */
    template <BufferType BT, UsageType UT = UsageType::Static>
    class Buffer {
    public:

        /**
         * Constructor for a buffer
         * @param data (const void*) Pointer to the data to but in the buffer
         * @param size (GLuint) Size in bytes of the buffer
         */
        Buffer(const void* data, GLuint size):
                m_RendererId(0), m_Count(size) {
            glCreateBuffers(1, &m_RendererId);

            bind();
            glBufferData(BT, size, data, UT);
            unBind();
        }

        /**
         * Destructor for the buffer,
         * Releases allocated resources
         */
        ~Buffer(){
            glDeleteBuffers(1, &m_RendererId);
        }

        /**
         * Binds the buffer to the GPU
         */
        void bind() const {
            glBindBuffer(BT, m_RendererId);
        }

        /**
         * Unbinds the buffer from the GPU
         */
        void unBind() const {
            glBindBuffer(BT, m_RendererId);
        }

        /**
         * Sets the data in the GPU buffer
         * @param data (const void*) Pointer to the data to set
         * @param size (GLuint) Size in bytes of the new buffer
         */
        void setData(const void* data, GLuint size) const {
            if (UT == UsageType::Static)
                std::cout << "Warning, dynamically modifying a static buffer, reconsider type if necessary." << std::endl;

            bind();
            glBufferData(BT, size, data, UT);
            unBind();

        }

        /**
         * Returns the number of indices if this buffer is an index buffer
         * @return (GLsizei) Number of indices
         */
        GLsizei getCount() const {
            static_assert(BT ==  BufferType::Index, "Cannot get count on a non-index buffer");
            return m_Count / sizeof(GLuint);
        }


    private:
        GLuint m_RendererId;
        uint32_t m_Count;

    };


} // OGLR::Buffers

