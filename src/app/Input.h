#pragma once

#include <utility>

#include "Application.h"

namespace OGLR::INPUT
{
    /*
    static bool IsKeyPressedImpl(int keycode);

    static bool IsMouseButtonPressedImpl(int button);
    static std::pair<float, float> GetMousePositionImpl();
    static float GetMouseXImpl();
    static float GetMouseYImpl();*/

    bool IsKeyPressedImpl(int keycode);

    bool IsMouseButtonPressedImpl(int button);

    std::pair<float, float> GetMousePositionImpl();
    float GetMouseXImpl();
    float GetMouseYImpl();
}
