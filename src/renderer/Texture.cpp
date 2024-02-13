#include <iostream>
#include "Texture.h"

#include "stb_image.h"

namespace OGLR
{


    Texture::Texture(const void* data, Type t, uint32_t width, uint32_t height):
        m_RendererID(0), m_Type(t), m_LocalBuffer(nullptr), m_Height(0), m_Width(0), m_BPP(0) {

        bool loadFromPath = width == 0 && height == 0;

        if (loadFromPath) {
            m_FilePath = (char*)data;

            stbi_set_flip_vertically_on_load(1);
            m_LocalBuffer = (void*)stbi_load(m_FilePath.c_str(), (int*)&m_Width, (int*)&m_Height, (int*)&m_BPP, 0);

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



    Texture::Texture(Texture &&other) noexcept
    : m_RendererID(other.m_RendererID), m_FilePath(std::move(other.m_FilePath)), m_LocalBuffer(other.m_LocalBuffer),
        m_Width(other.m_Width), m_Height(other.m_Height), m_BPP(other.m_BPP), m_Type(other.m_Type) {
        other.m_RendererID = 0;
        other.m_LocalBuffer = nullptr;
        std::cout << "Moved texture n°" << m_RendererID << std::endl;
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
                case Type::X4f:
                    delete (float*) m_LocalBuffer;
            }
	}

	void Texture::bind(GLuint slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Texture::unBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

    void Texture::bindAsImage(uint32_t slot, GLuint accessType) const {
        uint32_t format;
        switch(m_Type) {
            case Type::X4B:
                format =  GL_RGBA8;
                break;
            case Type::X1f:
                format = GL_R32F;
                break;
            case Type::X3f:
                throw std::runtime_error("Unsupported format");
            case Type::X4f:
                format = GL_RGBA32F;
                break;
        }

        glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, accessType, format);
    }

}

