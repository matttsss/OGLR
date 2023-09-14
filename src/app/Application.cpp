#include "Application.h"

#include <iostream>
#include <chrono>

#include "../utils/debug.h"

namespace OGLR
{

    GLFWwindow* Application::s_Window;

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

        s_Window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

        if (!s_Window)
            throw std::runtime_error("GLFW window did not get created... ");

        glfwMakeContextCurrent(s_Window);
        glfwSwapInterval(1); // Enable vsync

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_Io = &ImGui::GetIO(); (void)m_Io;

        m_Io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        m_Io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
        m_Io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (m_Io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        
        // Setup Platform/Renderer backends
        if (glewInit() != GLEW_OK)
            throw std::runtime_error("Glew initialisation failed... ");

        ImGui_ImplGlfw_InitForOpenGL(s_Window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

    }

    Application::~Application()
    {

        for (Layer* layer : m_Layers)
        {
            layer->onDettach();
            delete layer;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(s_Window);
        glfwTerminate();
    }

    void Application::run()
    {

        //std::chrono::
        while (!glfwWindowShouldClose(s_Window))
        {
            // Poll and handle Events
            glfwPollEvents();
            // TODO: handle events

            glClear(GL_COLOR_BUFFER_BIT);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            for (Layer* layer : m_Layers)
            {
                layer->onRender();

            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
            if (m_Io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            glfwSwapBuffers(s_Window);

        }
    }

    void Application::pushLayer(Layer* layer)
    {
        layer->onAttach();
        m_Layers.push_back(layer);
    }

    GLFWwindow *Application::getWindow() {
        return s_Window;
    }


} // OGLR