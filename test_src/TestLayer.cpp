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

    mesh = mcb.build();

}

void TestLayer::onRender() {

    m_Renderer.setCamera(m_Camera);
    m_Renderer.render(mesh, glm::mat4{1.0f});

    if (OGLR::INPUT::IsKeyPressedImpl(GLFW_KEY_A))
        std::cout << "Hello A " << std::endl;

    ImGui::ShowDemoWindow(nullptr);
}

void TestLayer::onUpdate(float dt)
{

}

void TestLayer::onDettach()
{
    delete mesh;
}
