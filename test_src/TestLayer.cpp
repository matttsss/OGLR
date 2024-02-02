#include "TestLayer.h"

#include <glm/gtc/type_ptr.hpp>

void TestLayer::onAttach()
{

    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.1f, 10.f);

    mesh = OGLR::MeshComponent::loadFromObjFile("test_res/models/tex_cube.obj")
            ->addShader("test_res/shaders/textured_simple.vert.glsl", "test_res/shaders/textured_simple.frag.glsl")
            ->addTexture("test_res/textures/tex_cube.png");

    terrain = OGLR::Terrain::buildTerrain(16, 1);
    terrain->addShader("test_res/shaders/textured_simple.vert.glsl", "test_res/shaders/textured_simple.frag.glsl");

}

void TestLayer::onRender() {


    glm::vec3 oldPos = position;
    //glm::vec3 oldScale = scale;
    ImGui::Begin("Renderer settings");

        ImGui::SliderFloat3("Cube position", glm::value_ptr(position), -2.00f, 2.0f);
        //ImGui::SliderFloat3("Cube scale", glm::value_ptr(scale), 0.00f, 3.0f);

    ImGui::End();

    transform = glm::translate(transform, position-oldPos);
    //transform = glm::scale(transform, scale-oldScale);

    m_Renderer.setCamera(m_Camera);
    m_Renderer.render(mesh, transform);
    m_Renderer.render(terrain, glm::mat4(1.0f));

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
