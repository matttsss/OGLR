#pragma once

namespace OGLR::DEBUG
{

	enum PriorityLevel
	{
		NONE = -1, NOTIFICATION = 0, LOW = 1, MEDIUM = 2, HIGH = 3
	};

	void setDebugPriorityLevel(PriorityLevel level);
	
    void glfwDebugCallback(int error, const char* description);

	void openGLDebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
	void enableGLDebug();
}