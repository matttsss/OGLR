#include "TestLayer.h"

#include <glm/gtc/type_ptr.hpp>

#include "utils/Terrain.h"

void TestLayer::onAttach()
{
    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.1f, 20.f);

    mesh = OGLR::MeshComponent::loadFromObjFile("test_res/models/tex_cube.obj")
            ->addShader("test_res/shaders/textured_simple.vert.glsl", "test_res/shaders/textured_simple.frag.glsl")
            ->addTexture("test_res/textures/tex_cube.png", OGLR::Texture::Type::X4B);

    terrain.addShader("test_res/shaders/terrain_shader.vert.glsl", "test_res/shaders/terrain_shader.frag.glsl");

}

void TestLayer::onRender() {
    ImGui::Begin("Renderer settings");

        ImGui::Text("Average render time (ms): %f", avrgFrameTime);
        ImGui::Checkbox("Render cube", &renderCube);

        ImGui::InputInt("Tile radius", (int*)&tSettings.radius);
        ImGui::InputInt("Mesh resolution", (int*)&tSettings.resolution);
        ImGui::SliderInt("Number of iterations", (int*)&tSettings.iter, 0, 128);
        ImGui::SliderAngle("Terrain offset angle", &tSettings.angle, 0, 2 * 360);

    ImGui::End();

    m_Renderer.setCamera(m_Camera);
    if (renderCube)
        m_Renderer.render(mesh, glm::mat4(1.0f));

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
