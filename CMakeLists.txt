cmake_minimum_required(VERSION 3.26)
project(OGLR)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME}
        src/MeshComponent.cpp
        src/Renderer.cpp
        src/Shader.cpp
        src/Texture.cpp
        src/buffers/IndexBuffer.cpp
        src/buffers/VertexArray.cpp
        src/buffers/VertexBuffer.cpp
        src/buffers/VertexBufferLayout.cpp
        src/vendor/stb_image/stb_image.cpp)

add_subdirectory(ext/glew-cmake)
add_subdirectory(ext/glfw)
add_subdirectory(ext/glm)


target_include_directories(${PROJECT_NAME} PUBLIC ext/stb)

target_link_libraries(${PROJECT_NAME} glfw glm libglew_static)