#pragma once

#include <GL/glew.h>

namespace OGLR::DEBUG
{

	enum PriorityLevel
	{
		NONE = -1, NOTIFICATION = 0, LOW = 1, MEDIUM = 2, HIGH = 3
	};

	void setDebugPriorityLevel(PriorityLevel level);
	

	void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	void enableGLDebug();
}