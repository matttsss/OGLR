#include "Renderer.h"

namespace OGLR
{

	void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << "[OpenGL Debug HIGH] " << message << std::endl;
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout << "[OpenGL Debug MEDIUM] " << message << std::endl;
			break;
		case GL_DEBUG_SEVERITY_LOW:
			std::cout << "[OpenGL Debug LOW] " << message << std::endl;
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			std::cout << "[OpenGL Debug NOTIFICATION] " << message << std::endl;
			break;
		}
	}

	Renderer::Renderer()
	{

#ifdef _DEBUG
		glDebugMessageCallback(openGLDebugCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::render(const MeshComponent& mesh, const glm::mat4& transform)
	{
		mesh.shader->bind();
		mesh.va->bind();
		mesh.ib->bind();

		if (mesh.texture)
			mesh.texture->bind();

		mesh.shader->setUniformMat4f("u_MVP", transform);

		glDrawElements(GL_TRIANGLES, mesh.ib->getCount(), GL_UNSIGNED_INT, nullptr);
	}

}