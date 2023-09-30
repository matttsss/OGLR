#pragma once

#include <GL/glew.h>

namespace OGLR::Buffers
{

	class IndexBuffer
	{
	public:
		IndexBuffer(const GLuint* indices, GLsizei count);
		~IndexBuffer();

		void bind() const;
		void unBind() const;

		const GLsizei& getCount() const { return m_Count; }

	private:
		GLuint m_RendererID;
        GLsizei m_Count;

	};

}