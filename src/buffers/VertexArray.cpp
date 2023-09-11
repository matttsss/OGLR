#include "VertexArray.h"

namespace OGLR::Buffers
{

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::bindAttributes(const VertexBuffer& vb, const VertexBufferLayout& bufferLayout) const
	{
		vb.bind();

		const auto& elements = bufferLayout.getAttributes();
		uint32_t offset = 0;

		for (int i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalised, bufferLayout.getStride(), (const void*)offset);
			offset += element.count * VertexBufferLayout::VertexAttribute::getSizeOfType(element.type);
		}

		vb.unbind();

	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::unBind() const
	{
		glBindVertexArray(0);
	}

}