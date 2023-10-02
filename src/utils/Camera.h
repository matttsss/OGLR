#pragma once

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace OGLR
{

    class Camera {
    public:

        Camera(glm::vec3 position, glm::vec3 aim)
        : position(position), aim(aim) {}

        void setOrthographicProjection(
            float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        void onUpdate(float dt);

        void setViewDirection(
            glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.f, 1.f, 0.f });
        void setViewTarget(
            glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.f, 1.f, 0.f });
        void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

        const glm::mat4& getProjection() const { return projectionMatrix; }
        const glm::mat4& getView();

    private:
        glm::mat4 projectionMatrix{ 1.f };
        glm::mat4 viewMatrix{ 1.f };

        glm::vec3 aim{ 0.0f, 0.0f, 1.0f };
        glm::vec3 position{ 0.f };

        bool hasMoved = true;
        bool mouseIsMoving = false;
        std::pair<float, float> lastMousePos = {0, 0};

    };


}