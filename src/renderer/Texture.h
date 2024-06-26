#pragma once

#include "GL/glew.h"

#include <string>

namespace OGLR
{

	class Texture
	{

	public:

        enum Type {
            X4f = GL_RGBA32F,
            X3f = GL_RGB32F,
            X1f = GL_R32F,
            X4B = GL_RGBA8
        };

        Texture() = delete;
        Texture(const Texture&) = delete;

        /***
         * Builds a texture interpreting data as a file path if width and height are both 0
         * Or interprets data as a pointer to the data itself if width and height are not 0
         * @param data (void*) Pointer to string or texture data
         * @param t (Type) Format of the texture
         * @param width
         * @param height
         */
        Texture(const void* data, Type t, uint32_t width = 0, uint32_t height = 0);
        Texture(Texture&& other) noexcept;
		~Texture();

		void bind(GLuint slot = 0) const;
        void bindAsImage(uint32_t slot, GLuint accessType) const;
		static void unBind();

        inline uint32_t getRendererID() const { return m_RendererID; }
		inline uint32_t getWidth() const { return m_Width; }
		inline uint32_t getHeight() const { return m_Height; }

	private:
		GLuint m_RendererID;
		std::string m_FilePath;
        Type m_Type;
		void* m_LocalBuffer;
		uint32_t m_Width, m_Height, m_BPP;

	};

}


