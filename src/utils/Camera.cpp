#include <iostream>
#include "../app/Input.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

namespace OGLR
{

    static const glm::vec3 s_UP = {0.0f, 1.0f, 0.0f};

	void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
        projectionMatrix = glm::perspective(fovy, aspect, near, far);
	}


    void Camera::onUpdate(float dt)
    {

        const glm::vec3 side = glm::cross(s_UP, aim);

        if (Input::IsKeyPressed(GLFW_KEY_W))
        {
            position += dt * mvmtSpeed * aim;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_S))
        {
            position -= dt * mvmtSpeed * aim;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_D))
        {
            position -= dt * mvmtSpeed * side;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            position += dt * mvmtSpeed * side;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
        {
            position -= dt * mvmtSpeed * s_UP;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
        {
            position += dt * mvmtSpeed * s_UP;
            hasMoved = true;
        }

        if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
        {
            if (!mouseIsMoving)
            {
                lastMousePos = Input::GetMousePosition();
                Input::LockMouseCursor(true);
                mouseIsMoving = true;
                return;
            }

            const std::pair<float, float> newPos = Input::GetMousePosition();

            // Moves sideways
            aim = glm::rotate(aim, (newPos.first - lastMousePos.first) * panSpeed * dt, s_UP);

            // Moves up/down
            const float dy = lastMousePos.second - newPos.second;
            glm::vec3 newAim = glm::rotate(aim, dy * panSpeed * dt, side);
            const float dot =  glm::dot(newAim, s_UP);

            // GimBall lock prevention check
            if (fabsf(dot) < 1 - 1e-4f)
                aim = newAim;

            hasMoved = true;
            lastMousePos = newPos;

        }
        else
        {
            Input::LockMouseCursor(false);
            mouseIsMoving = false;
        }

    }

    const glm::mat4 &Camera::getView()
    {
        if (hasMoved)
        {
            aim = glm::normalize(aim);
            viewMatrix = glm::lookAt(position, aim + position, s_UP);
            hasMoved = false;
        }

        return viewMatrix;
    }

}