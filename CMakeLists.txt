cmake_minimum_required(VERSION 3.26)
project(OGLR)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        src/main.cpp
        include/OGR.h
        src/MeshComponent.cpp
        src/Renderer.cpp
        src/Shader.cpp
        src/Texture.cpp
        src/Buffers/IndexBuffer.cpp
        src/Buffers/VertexArray.cpp
        src/Buffers/VertexBuffer.cpp
        src/Buffers/VertexBufferLayout.cpp)

add_subdirectory(ext/glfw)
add_subdirectory(ext/glm)
add_subdirectory(ext/glew-cmake)

target_link_libraries(${PROJECT_NAME} glfw glm libglew_static)