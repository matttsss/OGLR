#include "Input.h"

#include "Application.h"
#include <GLFW/glfw3.h>


namespace OGLR
{

    bool Input::IsKeyPressed(int keycode)
    {
        auto state = glfwGetKey(Application::getWindow(), keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        return glfwGetMouseButton(Application::getWindow(), button) == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos( Application::getWindow(), &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().first;
    }

    float Input::GetMouseY()
    {
        return GetMousePosition().second;
    }

}
