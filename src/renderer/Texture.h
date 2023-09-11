#pragma once

#include "GL/glew.h"

#include <string>

namespace OGLR
{

	class Texture
	{

	public:
		Texture(const std::string& path);
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


