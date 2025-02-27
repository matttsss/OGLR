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

        void setPerspectiveProjection(float fovy, float aspect, float near, float far);
        void setSpeed(float movementSpeed, float panningSpeed) { mvmtSpeed = movementSpeed; panSpeed = panningSpeed; }

        void onUpdate(float dt);

        const glm::vec3& getPosition() const { return position; }
        const glm::vec3& getAim() const { return aim; }
        const glm::mat4& getProjection() const { return projectionMatrix; }
        const glm::mat4& getView();

        static constexpr glm::vec3 s_UP = {0.0f, 1.0f, 0.0f};

    private:

        float mvmtSpeed = 1e-2;
        float panSpeed = 1e-3;

        glm::mat4 projectionMatrix{ 1.f };
        glm::mat4 viewMatrix{ 1.f };

        glm::vec3 aim{ 0.0f, 0.0f, 1.0f };
        glm::vec3 position{ 0.f };

        bool hasMoved = true;
        bool mouseIsMoving = false;
        std::pair<float, float> lastMousePos = {0, 0};

    };


}