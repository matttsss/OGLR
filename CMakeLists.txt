cmake_minimum_required(VERSION 3.26)
project(OGLR)

set(CMAKE_CXX_STANDARD 17)

#if (MSVC)
#    add_compile_options(/std:c++17)
#endif()


add_library(${PROJECT_NAME}
        ext/stb_image/stb_image.cpp
        src/renderer/MeshComponent.cpp
        src/renderer/Renderer.cpp
        src/renderer/Shader.cpp
        src/renderer/Texture.cpp
        src/buffers/VertexArray.cpp
        src/utils/debug.cpp
        src/utils/Camera.cpp
        src/utils/Terrain.cpp
        src/app/Application.cpp
        src/app/Input.cpp
        src/renderer/Shader_Unif_Impl.cpp
        src/buffers/VertexArray_Attrib_Impl.cpp
        src/buffers/Buffer.cpp
)

add_executable(${PROJECT_NAME}_TEST
        test_src/main.cpp
        test_src/TestLayer.cpp
)

# Copies the resource folder to the target directory for compatibility
# with different build systems
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
#                   ${CMAKE_COMMAND} -E copy_directory
#                       ${CMAKE_SOURCE_DIR}/test_res $<TARGET_FILE_DIR:${PROJECT_NAME}>/test_res)

add_subdirectory(ext)

target_include_directories(${PROJECT_NAME} PUBLIC ${EXT_INCLUDE_DIR} src)
target_include_directories(${PROJECT_NAME}_TEST PUBLIC include)

target_link_libraries(${PROJECT_NAME} EXT_LIB)
target_link_libraries(${PROJECT_NAME}_TEST ${PROJECT_NAME})