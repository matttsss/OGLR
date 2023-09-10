//
// Created by matts on 10/09/2023.
//

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

int main(void)
{

    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
}