#include "TestLayer.h"

void TestLayer::onAttach()
{
    OGLR::Buffers::VertexBufferLayout vbl;
    vbl.addFloat(2);
    vbl.addFloat(2);

    OGLR::MeshComponent::MeshComponentBuilder mcb;
    mcb.setVertices(vertices, 4 * 4 * sizeof(GLfloat));
    mcb.setIndices(indices, 6);
    mcb.setVerticesLayout(vbl);
    mcb.setShaderPath("test_res/shaders/test.vert.glsl", "test_res/shaders/test.frag.glsl");
    mcb.setTexturePath("test_res/textures/mountains.png");

    m_Camera.setOrthographicProjection(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);

    mesh = mcb.build();

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
