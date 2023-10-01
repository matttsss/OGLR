#include "TestLayer.h"

void TestLayer::onAttach()
{

    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.1f, 10.f);

    mesh = OGLR::MeshComponent::loadFromFiles(
        "test_res/models/colored_cube.obj",
        "test_res/shaders/colored_obj");

}

void TestLayer::onRender() {

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
