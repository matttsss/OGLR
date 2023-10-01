#include "TestLayer.h"

#include "../src/buffers/Buffer.h"

void TestLayer::onAttach()
{

    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.1f, 10.f);

    mesh = OGLR::MeshComponent::loadFromObjFile("test_res/models/tex_cube.obj")
            ->addShader("test_res/shaders/textured_simple.vert.glsl", "test_res/shaders/textured_simple.frag.glsl")
            ->addTexture("test_res/textures/tex_cube.png");

}

void TestLayer::onRender() {



    ImGui::Begin("Renderer settings");



    ImGui::End();

    m_Renderer.setCamera(m_Camera);
    m_Renderer.render(mesh, glm::mat4{1.0f});

    ImGui::ShowDemoWindow(nullptr);
}

void TestLayer::onUpdate(float dt)
{
    m_Camera.onUpdate(dt);
}

void TestLayer::onDettach()
{
    delete mesh;
}
