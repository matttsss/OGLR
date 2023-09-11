#pragma once

#include <GL/glew.h>

namespace OGLR::Buffers
{

	class IndexBuffer
	{
	public:
		IndexBuffer(const GLuint* indices, GLuint count);
		~IndexBuffer();

		void bind() const;
		void unBind() const;

		const GLuint& getCount() const { return m_Count; }

	private:
		GLuint m_RendererID;
		GLuint m_Count;

	};

}