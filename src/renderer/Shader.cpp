#include "Shader.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace OGLR
{

	static std::string ReadFileAsString(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize((size_t)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			std::cout << "Could not find file : " << filepath << std::endl;
		}

		return result;
	}

    Shader::Shader(Shader &&other) noexcept
            : m_RendererID(other.m_RendererID),
              m_FirstPath(std::move(other.m_FirstPath)),
              m_SecondPath(std::move(other.m_SecondPath)),
              m_UniformLocationCache(std::move(other.m_UniformLocationCache)) {
        other.m_RendererID = 0;
    }

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::unBind()
	{
		glUseProgram(0);
	}

	Shader* Shader::fromGLSLTextFiles(const std::string& firstShaderPath, const std::string& secondShaderPath)
	{
		Shader* shader = new Shader();
        shader->m_FirstPath = firstShaderPath;
        shader->m_SecondPath = secondShaderPath;
        if (secondShaderPath.empty())
            shader->loadComputeFromFile();
        else
		    shader->loadFromGLSLTextFiles();

        Shader::unBind();
		return shader;
	}

    void Shader::loadFromGLSLTextFiles()
	{
		std::string vertexSource = ReadFileAsString(m_FirstPath);
		std::string fragmentSource = ReadFileAsString(m_SecondPath);

		GLuint program = glCreateProgram();

        std::cout << "Compiling vertex shader: " << m_FirstPath << std::endl;
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
		glAttachShader(program, vertexShader);

        std::cout << "Compiling fragment shader: " << m_SecondPath << std::endl;
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			std::cerr << "Error while compiling shader : " << infoLog.data() << std::endl;
            exit(1);
		}
		
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_RendererID = program;
	}


    void Shader::loadComputeFromFile() {
        std::string computeSource = ReadFileAsString(m_FirstPath);

        GLuint program = glCreateProgram();

        std::cout << "Compiling compute shader: " << m_FirstPath << std::endl;
        GLuint computeShader = compileShader(GL_COMPUTE_SHADER, computeSource);
        glAttachShader(program, computeShader);

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            glDeleteShader(computeShader);

            std::cerr << "Error while compiling shader : " << infoLog.data() << std::endl;
            exit(1);
        }

        glDetachShader(program, computeShader);
        glDeleteShader(computeShader);

        m_RendererID = program;
    }


    GLuint Shader::compileShader(GLenum type, const std::string& source)
    {
        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, nullptr);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            std::cout << "Error while compiling shader " << source << ": \n" << infoLog.data() << std::endl;
        }

        return shader;
    }

}