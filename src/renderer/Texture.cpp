#include <iostream>
#include "Texture.h"

#include "stb_image.h"

namespace OGLR
{


    Texture::Texture(const void* data, Type t, int32_t width, int32_t height):
        m_RendererID(0), m_Type(t), m_LocalBuffer(nullptr), m_Height(0), m_Width(0), m_BPP(0) {

        bool loadFromPath = width == 0 && height == 0;

        if (loadFromPath) {
            m_FilePath = (char*)data;

            stbi_set_flip_vertically_on_load(1);
            m_LocalBuffer = (void*)stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 0);

        } else {

            m_LocalBuffer = (void*)data;
            m_Height = height;
            m_Width = width;

        }

        //if (!m_LocalBuffer)
        //    throw std::runtime_error("Unable to load texture data");

        GLint internalFormat, format, type;
        switch(m_Type) {
            case Type::X4B:
                internalFormat = GL_RGBA8;
                format         = GL_RGBA;
                type           = GL_UNSIGNED_BYTE;
                break;
            case Type::X1f:
                internalFormat = GL_R32F;
                format         = GL_RED;
                type           = GL_FLOAT;
                break;
            case Type::X3f:
                internalFormat = GL_RGB32F;
                format         = GL_RGB;
                type           = GL_FLOAT;
                break;
            case Type::X4f:
                internalFormat = GL_RGBA32F;
                format         = GL_RGBA;
                type           = GL_FLOAT;
                break;
        }

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, type, m_LocalBuffer);

        unBind();

        if (loadFromPath && m_LocalBuffer)
            stbi_image_free(m_LocalBuffer);


    }



    Texture::Texture(Texture &&other)
        : m_RendererID(other.m_RendererID), m_FilePath(other.m_FilePath), m_LocalBuffer(other.m_LocalBuffer),
        m_Width(other.m_Width), m_Height(other.m_Height), m_BPP(other.m_BPP){
        other.m_RendererID = 0;
        other.m_LocalBuffer = nullptr;
    }

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_RendererID);
        if (m_LocalBuffer && m_FilePath.empty())
            switch (m_Type) {
                case Type::X4B:
                    delete (unsigned char*) m_LocalBuffer;
                    break;
                case Type::X1f:
                case Type::X3f:
                    delete (float*) m_LocalBuffer;
            }
	}

	void Texture::bind(GLuint slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Texture::unBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}

