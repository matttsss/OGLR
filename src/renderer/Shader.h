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


		GLuint getRendererID() const { return m_RendererID; }

        /***
         * Creates a shader with the given file paths,
         * If the second shader path is empty, it will try to create a compute shader
         * @param firstShaderPath (std::string) Vertex or Compute shader path
         * @param secondShaderPath (std::string) Fragment shader path, keep empty if trying to
         * make a compute shader
         * @return (Shader*)
         */
		static Shader* FromGLSLTextFiles(const std::string& firstShaderPath, const std::string& secondShaderPath = "");

	private:
		Shader() = default;

		GLint getUniformLocation(const std::string& name);

		void loadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        void loadComputeFromFile(const std::string& computeShaderPath);
		GLuint CompileShader(GLenum type, const std::string& source);

	private:

		GLuint m_RendererID;
        std::string m_FirstPath, m_SecondPath;
		std::unordered_map<std::string, GLint> m_UniformLocationCache;

	};

}