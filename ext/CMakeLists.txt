cmake_minimum_required(VERSION 3.26)
project(EXT_LIB)

set(CMAKE_CXX_STANDARD 14)

set(IMGUI_SRC
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

add_library(${PROJECT_NAME}
        stb_image/stb_image.cpp
        tinyobjloader/tiny_obj_loader.cpp
        ${IMGUI_SRC}
)

add_subdirectory(glew-cmake)
add_subdirectory(glfw)
add_subdirectory(glm)

set(EXT_INCLUDE_DIR ext/tinyobjloader ext/stb_image ext/imgui ext/imgui/backends PARENT_SCOPE)

target_include_directories(${PROJECT_NAME} PUBLIC stb_image imgui imgui/backends)

target_link_libraries(${PROJECT_NAME} glfw glm libglew_static)