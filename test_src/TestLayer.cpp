#include "TestLayer.h"

void TestLayer::onAttach()
{
    OGLR::Buffers::VertexBufferLayout vbl;
    vbl.addFloat(3);
    vbl.addFloat(2);

    m_Camera.setOrthographicProjection(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 1.0f);

    mesh = new OGLR::MeshComponent{
        "test_res/models/colored_cube.obj",
        "test_res/shaders/colored_obj"};


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
