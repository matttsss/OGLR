#pragma once

#include <string>
#include <stdexcept>
#include <unordered_map>

#include "GL/glew.h"

#include "../buffers/Buffer.h"

namespace OGLR
{

	class Shader
	{
	public:

        Shader(const Shader&) = delete;
        Shader(Shader&& other) noexcept;
		~Shader();

		void bind() const;
		static void unBind();

        template<typename T>
        void setUniform(const std::string& name, const T& val);

        void setUniformBlock(const std::string& name, const Buffer& ubo);


		GLuint getRendererID() const { return m_RendererID; }

        /***
         * Creates a shader with the given file paths,
         * If the second shader path is empty, it will try to create a compute shader
         * @param firstShaderPath (std::string) Vertex or Compute shader path
         * @param secondShaderPath (std::string) Fragment shader path, keep empty if trying to
         * make a compute shader
         * @return (Shader*)
         */
		static Shader* fromGLSLTextFiles(const std::string& firstShaderPath, const std::string& secondShaderPath = "");

	private:
		Shader() = default;

		GLint getUniformLocation(const std::string& name);
		GLint getUniformBlockIdx(const std::string& name);
		GLuint getUniformBlockBindingPoint(const std::string& name);

		void loadFromGLSLTextFiles();
        void loadComputeFromFile();
		static GLuint compileShader(GLenum type, const std::string& source);

	private:

		GLuint m_RendererID;
        std::string m_FirstPath, m_SecondPath;

        GLuint m_NxtAvailableBindingPt = 0;
		std::unordered_map<std::string, GLint> m_UniformLocationCache;
		std::unordered_map<std::string, GLuint> m_UniformBindingPoints;

	};

}