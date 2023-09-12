#include "debug.h"

#include <iostream>

#include <GL/glew.h>

namespace OGLR::DEBUG
{

	static PriorityLevel s_PriorityLevel = PriorityLevel::HIGH;	


	void setDebugPriorityLevel(PriorityLevel level)
	{
		s_PriorityLevel = level;
	}

    void glfwDebugCallback(int error, const char* description)
    {
        std::cout << "GLFW error " << error << ": " << description << std::endl;
    }

	void openGLDebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
	{
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				if (s_PriorityLevel >= PriorityLevel::HIGH)
					std::cout << "[OpenGL Debug HIGH] " << message << std::endl;
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				if (s_PriorityLevel >= PriorityLevel::MEDIUM)
					std::cout << "[OpenGL Debug MEDIUM] " << message << std::endl;
				break;
			case GL_DEBUG_SEVERITY_LOW:
				if (s_PriorityLevel >= PriorityLevel::LOW)
					std::cout << "[OpenGL Debug LOW] " << message << std::endl;
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				if (s_PriorityLevel >= PriorityLevel::NOTIFICATION)
					std::cout << "[OpenGL Debug NOTIFICATION] " << message << std::endl;
				break;
			}
		}
	}

	void enableGLDebug()
	{
		glDebugMessageCallback(openGLDebugCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

}

