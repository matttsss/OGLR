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
        ext/stb_image/stb_image.cpp
        src/utils/debug.cpp
        src/Camera.h
        src/Camera.cpp)

add_executable(${PROJECT_NAME}_TEST
        test_src/main.cpp)

add_subdirectory(ext)

target_include_directories(${PROJECT_NAME} PUBLIC ${EXT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME}_TEST PUBLIC include)

target_link_libraries(${PROJECT_NAME} EXT_LIB)
target_link_libraries(${PROJECT_NAME}_TEST ${PROJECT_NAME})