#include "Application.h"

#include <iostream>
#include <chrono>

#include "../utils/debug.h"

namespace OGLR
{

    Application::Application(const std::string& windowName, int width, int height)
    {

        glfwSetErrorCallback(DEBUG::glfwDebugCallback);
        if (!glfwInit())
            throw std::runtime_error("GLFW did not initialise... ");

        const char* glsl_version = "#version 430 core";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

        if (!window)
            throw std::runtime_error("GLFW window did not get created... ");

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO(); (void)io;

        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
        io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

    }

    Application::~Application()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Application::run()
    {

        //std::chrono::
        while (!glfwWindowShouldClose(window))
        {
            // Poll and handle Events
            glfwPollEvents();
            // TODO: handle events

            glClear(GL_COLOR_BUFFER_BIT);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            for (Layer& layer : layers)
            {
                layer.onRender();

            }

            ImGui::ShowDemoWindow(nullptr);


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
            if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            glfwSwapBuffers(window);

        }
    }


} // OGLR