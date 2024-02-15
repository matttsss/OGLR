#include "TestLayer.h"

#include <glm/gtc/type_ptr.hpp>

#include "utils/Terrain.h"

void TestLayer::onAttach()
{
    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.1f, 20.f);

    mesh = OGLR::MeshComponent::loadFromObjFile("test_res/models/tex_cube.obj")
            ->addShader("test_res/shaders/textured_simple.vert.glsl", "test_res/shaders/textured_simple.frag.glsl")
            ->addTexture("test_res/textures/tex_cube.png", OGLR::Texture::Type::X4B);

    terrain.renderShader = OGLR::Shader::fromGLSLTextFiles("test_res/shaders/terrain_shader.vert.glsl", "test_res/shaders/terrain_shader.frag.glsl");

}

void TestLayer::onRender() {
    ImGui::Begin("Renderer settings");

        ImGui::Text("Average render time (ms): %f", avrgFrameTime);
        ImGui::Checkbox("Render cube", &renderCube);
        if (renderCube)
        {
            ImGui::SliderFloat3("Cube position", &transform[3].x, -5, 5);
        }

        ImGui::InputInt("Tile radius", (int*)&tSettings.radius);
        ImGui::InputInt("Mesh resolution", (int*)&tSettings.resolution);
        ImGui::SliderInt("Number of octaves", (int*)&tSettings.nbOctaves, 0, 128);
        ImGui::SliderFloat("Terrain offset angle", &tSettings.angle, 0, 2*glm::pi<float>());

    ImGui::End();

    m_Renderer.setCamera(m_Camera);
    if (renderCube)
        m_Renderer.render(mesh, transform);

    terrain.updateWithSettings(tSettings);
    m_Renderer.render(terrain);

    //ImGui::ShowDemoWindow(nullptr);
}

void TestLayer::onUpdate(float dt)
{

    m_Camera.onUpdate(dt);

    renderTimes[renderTimeIdx] = dt;
    renderTimeIdx = (renderTimeIdx + 1) % 100;
    if (renderTimeIdx == 0) {
        avrgFrameTime = 0;
        for (float t : renderTimes)
            avrgFrameTime += t;
        avrgFrameTime /= 100;

    }

}

void TestLayer::onDetach()
{
    OGLR::Terrain::destroyTerrain();
    delete mesh;
}
