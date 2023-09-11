#pragma once

#include <GL/glew.h>

#include <vector>
#include <iostream>


namespace OGLR::Buffers
{

	class VertexBufferLayout
	{
	public:
		struct VertexAttribute
		{
			VertexAttribute() = default;

			GLint count;
			GLenum type;
			GLboolean normalised;

			static GLuint getSizeOfType(GLenum type)
			{
				switch (type)
				{
				case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
				case GL_UNSIGNED_INT: return sizeof(GLuint);
				case GL_FLOAT: return sizeof(GLfloat);

				default:
					std::cout << "Incorect type passed to VertexAttibute::getSizeOfType... " << std::endl;
					return 0;
				}
			};

		};

		VertexBufferLayout() = default;

		inline const std::vector<VertexAttribute>& getAttributes() const { return m_Attributes; }
		inline const GLsizei& getStride() const { return m_Stride; }

		void addFloat(GLsizei count);

	private:

        GLsizei m_Stride = 0;
		std::vector<VertexAttribute> m_Attributes;

	};

}
