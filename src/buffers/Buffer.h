#pragma once

#include <GL/glew.h>
#include <iostream>

namespace OGLR {

    /// Indicates the type of the buffer
    enum BufferType {
        Idx = GL_ELEMENT_ARRAY_BUFFER, Vtx = GL_ARRAY_BUFFER, Unf = GL_UNIFORM_BUFFER
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

        Buffer() = delete;
        Buffer(Buffer&& other) noexcept
            : m_RendererID(other.m_RendererID),
              m_Count(other.m_Count) {
            other.m_RendererID = 0;
        }
        Buffer(const Buffer&) = delete;

        /**
         * Constructor for a buffer
         * @param data (const void*) Pointer to the data to but in the buffer
         * @param size (GLuint) Size in bytes of the buffer
         */
        Buffer(const void* data, GLuint size):
                m_RendererID(0), m_Count(size) {
            glCreateBuffers(1, &m_RendererID);

            bind();
            glBufferData(BT, size, data, UT);
            unBind();
        }

        /**
         * Destructor for the buffer,
         * Releases allocated resources
         */
        ~Buffer(){
            glDeleteBuffers(1, &m_RendererID);
        }

        /**
         * Binds the buffer to the GPU
         */
        void bind() const {
            glBindBuffer(BT, m_RendererID);
        }

        /**
         * Unbinds the buffer from the GPU
         */
        static void unBind() {
            glBindBuffer(BT, 0);
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
            static_assert(BT ==  BufferType::Idx, "Cannot get count on a non-index buffer");
            return m_Count / sizeof(GLuint);
        }


    private:
        GLuint m_RendererID;
        uint32_t m_Count;

    };

}
