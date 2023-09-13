#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>
#include <string>

#include "Layer.h"


namespace OGLR
{

    class Application
    {
    public:

        Application(const std::string& windowName, int width, int height);
        ~Application();

        void pushLayer(Layer* layer);

        void run();


    private:

        GLFWwindow* m_Window;
        ImGuiIO* m_Io;

        std::vector<Layer*> m_Layers;

    };

} // OGLR
