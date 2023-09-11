#include "VertexBuffer.h"

namespace OGLR::Buffers
{

	VertexBuffer::VertexBuffer(const void* data, GLuint size)
	{
		glCreateBuffers(1, &m_RendererID);
		int size_bis = sizeof(data);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		unbind();
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}