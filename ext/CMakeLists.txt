cmake_minimum_required(VERSION 3.26)
project(EXT_LIB)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME}
        stb_image/stb_image.cpp
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
)

add_subdirectory(glew-cmake)
add_subdirectory(glfw)
add_subdirectory(glm)

set(EXT_INCLUDE_DIR ext/stb_image ext/imgui ext/imgui/backends PARENT_SCOPE)

target_include_directories(${PROJECT_NAME} PUBLIC stb_image imgui imgui/backends)

target_link_libraries(${PROJECT_NAME} glfw glm libglew_static)