#include "UniformBuffer.h"

namespace OGLR::BUFFERS
{


    UniformBuffer::UniformBuffer(const void *data, GLuint size)
    {
        glGenBuffers(1, &m_RendererID);
        setData(data, size);
    }

    UniformBuffer::~UniformBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void UniformBuffer::bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    }

    void UniformBuffer::unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UniformBuffer::setData(const void *data, GLuint size)
    {
        bind();
        glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
        unbind();
    }


}