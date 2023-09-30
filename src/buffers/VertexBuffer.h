#pragma once

#include <GL/glew.h>

namespace OGLR::Buffers
{

	class VertexBuffer
	{

	public:
		VertexBuffer(const void* data, GLuint size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;

	private:
		GLuint m_RendererID;
	};

}