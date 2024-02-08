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


	void VertexArray::bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::unBind() const
	{
		glBindVertexArray(0);
	}

}