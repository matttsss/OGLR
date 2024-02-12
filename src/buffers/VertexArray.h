#pragma once

#include "Buffer.h"
#include "VertexBufferLayout.h"

namespace OGLR
{

	class VertexArray
	{
	public:

		VertexArray();
        VertexArray(VertexArray&& other) noexcept;
        VertexArray(const VertexArray&) = delete;
		~VertexArray();

        template <UsageType UT>
		void bindAttributes(const Buffer<BufferType::Vtx, UT>& vb, const VertexBufferLayout& bufferLayout) const {
            vb.bind();

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

            vb.unBind();

        }

		void bind() const;
		void unBind() const;

	private:
		GLuint m_RendererID;
	};

}