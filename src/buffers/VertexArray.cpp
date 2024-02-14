#include "VertexArray.h"

namespace OGLR
{

	VertexArray::VertexArray()
        : m_RendererID(0)
	{
		glGenVertexArrays(1, &m_RendererID);
	}

    VertexArray::VertexArray(VertexArray &&other) noexcept
        : m_RendererID(other.m_RendererID) {
        other.m_RendererID = 0;
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }


	void VertexArray::bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::unBind()
	{
		glBindVertexArray(0);
	}


}