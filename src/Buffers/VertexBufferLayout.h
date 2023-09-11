#pragma once

#include <GL/glew.h>

#include <vector>


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
					LOG_ERROR("Incorect type passed to VertexAttibute::getSizeOfType... ");
					return 0;
				}
			};

		};

		VertexBufferLayout() = default;

		inline const std::vector<VertexAttribute>& getAttributes() const { return m_Attributes; }
		inline const GLuint& getStride() const { return m_Stride; }

		void addFloat(GLint count);

	private:

		GLuint m_Stride = 0;
		std::vector<VertexAttribute> m_Attributes;

	};

}
