#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace OGLR::Buffers
{

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void bindAttributes(const VertexBuffer& vb, const VertexBufferLayout& bufferLayout) const;

		void bind() const;
		void unBind() const;

	private:
		GLuint m_RendererID;
	};

}