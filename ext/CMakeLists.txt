cmake_minimum_required(VERSION 3.26)
project(EXT_LIB)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME}
        stb_image/stb_image.cpp)

add_subdirectory(glew-cmake)
add_subdirectory(glfw)
add_subdirectory(glm)

set(EXT_INCLUDE_DIR ext/stb_image PARENT_SCOPE)

target_include_directories(${PROJECT_NAME} PUBLIC stb_image)

target_link_libraries(${PROJECT_NAME} glfw glm libglew_static)