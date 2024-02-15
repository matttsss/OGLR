#pragma once

#include <GL/glew.h>
#include <iostream>

namespace OGLR {

    /// Indicates the type of the buffer
    enum BufType {
        IBO = GL_ELEMENT_ARRAY_BUFFER, VBO = GL_ARRAY_BUFFER, UBO = GL_UNIFORM_BUFFER, SSBO = GL_SHADER_STORAGE_BUFFER
    };

    /// Indicates how the buffer will be used
    enum UsageType {
        Static = GL_STATIC_DRAW, Dynamic = GL_DYNAMIC_DRAW
    };


    class Buffer {
    public:

        Buffer() = delete;
        Buffer(Buffer&& other) noexcept;
        Buffer(const Buffer&) = delete;

        /**
         * Constructor for a buffer
         * @param data (const void*) Pointer to the data to but in the buffer
         * @param size (GLuint) Size in bytes of the buffer
         */
        Buffer(BufType BT, const void* data, GLuint size, UsageType UT = UsageType::Static);

        /**
         * Destructor for the buffer,
         * Releases allocated resources
         */
        ~Buffer();

        /**
         * Binds the buffer to the GPU
         */
        void bind() const;

        /**
         * Unbinds the buffer from the GPU
         */
        void unBind() const;

        /**
         * Sets the data in the GPU buffer
         * @param data (const void*) Pointer to the data to set
         * @param size (GLuint) Size in bytes of the new buffer
         */
        void setData(const void* data, GLuint size) const;

        void castTo(BufType BT, UsageType UT = UsageType::Static);

        inline GLuint getRendererID() const { return m_RendererID; }

        /**
         * Returns the number of indices if this buffer is an index buffer
         * @return (GLsizei) Number of indices
         */
        GLsizei getCount() const {
            if (m_BT != BufType::IBO) throw std::runtime_error("Illegal call, wrong buffer type");
            return m_Count / sizeof(GLuint);
        }


    private:
        GLuint m_RendererID;
        uint32_t m_Count;

        BufType m_BT;
        UsageType m_UT = UsageType::Static;
    };


}
