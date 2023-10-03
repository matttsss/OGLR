#include "../app/Input.h"
#include "Camera.h"

#include <glm/gtx/rotate_vector.hpp>

namespace OGLR
{

    static const glm::vec3 s_UP = {0.0f, 1.0f, 0.0f};

	void Camera::setOrthographicProjection(
		float left, float right, float top, float bottom, float near, float far) {
		projectionMatrix = glm::mat4{ 1.0f };
		projectionMatrix[0][0] = 2.f / (right - left);
		projectionMatrix[1][1] = 2.f / (bottom - top);
		projectionMatrix[2][2] = 1.f / (far - near);
		projectionMatrix[3][0] = -(right + left) / (right - left);
		projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		projectionMatrix[3][2] = -near / (far - near);
	}

	void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
		//assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tanf(fovy / 2.f);
		projectionMatrix = glm::mat4{ 0.0f };
		projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		projectionMatrix[1][1] = 1.f / (tanHalfFovy);
		projectionMatrix[2][2] = far / (far - near);
		projectionMatrix[2][3] = 1.f;
		projectionMatrix[3][2] = -(far * near) / (far - near);
	}

	void Camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
		const glm::vec3 w{ direction };
		const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
		const glm::vec3 v{ glm::cross(w, u) };

		viewMatrix = glm::mat4{ 1.f };
		viewMatrix[0][0] = u.x;
		viewMatrix[1][0] = u.y;
		viewMatrix[2][0] = u.z;
		viewMatrix[0][1] = v.x;
		viewMatrix[1][1] = v.y;
		viewMatrix[2][1] = v.z;
		viewMatrix[0][2] = w.x;
		viewMatrix[1][2] = w.y;
		viewMatrix[2][2] = w.z;
		viewMatrix[3][0] = -glm::dot(u, position);
		viewMatrix[3][1] = -glm::dot(v, position);
		viewMatrix[3][2] = -glm::dot(w, position);
	}

	void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
		setViewDirection(position, target - position, up);
	}

	void Camera::setViewYXZ(glm::vec3 position, glm::vec3 rotation) {
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
		const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
		const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
		viewMatrix = glm::mat4{ 1.f };
		viewMatrix[0][0] = u.x;
		viewMatrix[1][0] = u.y;
		viewMatrix[2][0] = u.z;
		viewMatrix[0][1] = v.x;
		viewMatrix[1][1] = v.y;
		viewMatrix[2][1] = v.z;
		viewMatrix[0][2] = w.x;
		viewMatrix[1][2] = w.y;
		viewMatrix[2][2] = w.z;
		viewMatrix[3][0] = -glm::dot(u, position);
		viewMatrix[3][1] = -glm::dot(v, position);
		viewMatrix[3][2] = -glm::dot(w, position);
	}

    void Camera::onUpdate(float dt)
    {

        const glm::vec3 side = glm::cross(s_UP, aim);

        if (Input::IsKeyPressed(GLFW_KEY_W))
        {
            position += dt * aim;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_S))
        {
            position -= dt * aim;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_D))
        {
            position -= dt * side;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            position += dt * side;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
        {
            position -= dt * s_UP;
            hasMoved = true;
        }
        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
        {
            position += dt * s_UP;
            hasMoved = true;
        }

        bool mouseButtonIsPressed = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1);
        if (mouseButtonIsPressed && !mouseIsMoving)
        {
            lastMousePos = Input::GetMousePosition();
            Input::LockMouseCursor(true);
            mouseIsMoving = true;
            return;
        }

        if (mouseButtonIsPressed && mouseIsMoving)
        {
            const std::pair<float, float> newPos = Input::GetMousePosition();

            // Moves sideways
            aim = glm::rotate(aim, (lastMousePos.first - newPos.first) * dt, s_UP);

            // Moves up/down
            const float dot =  glm::dot(aim, s_UP);
            const float dy = lastMousePos.second - newPos.second;
            if (!((dy > 0 && dot < -0.90) || (dy < 0 && dot > 0.90))) // TODO gimbal lock
                aim = glm::rotate(aim, dy * dt, side);
            hasMoved = true;

            lastMousePos = newPos;
            return;
        }

        Input::LockMouseCursor(false);
        mouseIsMoving = false;

    }

    const glm::mat4 &Camera::getView()
    {
        if (hasMoved)
        {
            aim = glm::normalize(aim);
            setViewDirection(position, aim);
            hasMoved = false;
        }

        return viewMatrix;
    }

}