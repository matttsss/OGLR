#pragma once

#include "GL/glew.h"

#include <string>

namespace OGLR
{

	class Texture
	{

	public:

        enum Type {
            X3f = GL_RGB32F,
            X1f = GL_R32F,
            X4B = GL_RGBA8
        };

		Texture(const std::string& path, Type t);
		~Texture();

		void bind(GLuint slot = 0) const;
		void unBind() const;

		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }

	private:
		GLuint m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;

	};

}


