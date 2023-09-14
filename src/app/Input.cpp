#include "Input.h"

#include "Application.h"
#include <GLFW/glfw3.h>


namespace OGLR::INPUT
{


    bool IsKeyPressedImpl(int keycode)
    {
        auto state = glfwGetKey(Application::getWindow(), keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool IsMouseButtonPressedImpl(int button)
    {
        return glfwGetMouseButton(Application::getWindow(), button) == GLFW_PRESS;
    }

    std::pair<float, float> GetMousePositionImpl()
    {
        double xpos, ypos;
        glfwGetCursorPos( Application::getWindow(), &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    float GetMouseXImpl()
    {
        return GetMousePositionImpl().first;
    }

    float GetMouseYImpl()
    {
        return GetMousePositionImpl().second;
    }


}
