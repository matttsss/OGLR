#include "IndexBuffer.h"

#include "VertexBufferLayout.h"

namespace OGLR::Buffers
{

	IndexBuffer::IndexBuffer(const GLuint* indices, GLuint count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);

		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);
		unBind();

	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::unBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}