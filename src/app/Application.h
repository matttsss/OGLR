#pragma once

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


        void run();


    private:

        GLFWwindow* window;
        ImGuiIO* io;

        std::vector<Layer> layers;

    };

} // OGLR
