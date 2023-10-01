#pragma once

#include <GL/glew.h>

namespace OGLR::BUFFERS
{

    class UniformBuffer {
    public:

        UniformBuffer() = delete;
        UniformBuffer(const void* data, GLuint size);

        ~UniformBuffer();

        void bind() const;
        void unbind() const;

        void setData(const void* data, GLuint size);


    private:

        GLuint m_RendererID;

    };


} // OGLR
