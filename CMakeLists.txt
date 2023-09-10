cmake_minimum_required(VERSION 3.26)
project(OGLR)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        src/main.cpp)

add_subdirectory(ext/glfw)
add_subdirectory(ext/glm)
add_subdirectory(ext/glew/build/cmake)

target_link_libraries(${PROJECT_NAME} glfw glm glew_s)