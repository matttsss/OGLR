#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "GL/glew.h"

namespace OGLR
{

	class Shader
	{
	public:
		~Shader();

		void bind() const;
		void unBind() const;

		void setUniform1i(const std::string& name, int v1);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void setUniformMat4f(const std::string& name, const glm::mat4& mat);


		GLuint GetRendererID() const { return m_RendererID; }

		static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	private:
		Shader() = default;

		GLint getUniformLocation(const std::string& name);

		void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		GLuint CompileShader(GLenum type, const std::string& source);

	private:

		GLuint m_RendererID;
		std::unordered_map<std::string, GLint> m_UniformLocationCache;

	};

}