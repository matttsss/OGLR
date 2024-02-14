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


    void VertexArray::bindAttributes(const VertexBufferLayout &bufferLayout) const {

        const auto& elements = bufferLayout.getAttributes();
        uint32_t offset = 0;

        for (int i = 0; i < elements.size(); ++i)
        {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalised, bufferLayout.getStride(),
                                  reinterpret_cast<const void *>(offset));
            offset += element.count * VertexBufferLayout::VertexAttribute::getSizeOfType(element.type);
        }

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