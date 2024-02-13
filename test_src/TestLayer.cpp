#include "TestLayer.h"

#include <glm/gtc/type_ptr.hpp>

#include "utils/Terrain.h"

void TestLayer::onAttach()
{
    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.1f, 10.f);

    mesh = OGLR::MeshComponent::loadFromObjFile("test_res/models/tex_cube.obj")
            ->addShader("test_res/shaders/textured_simple.vert.glsl", "test_res/shaders/textured_simple.frag.glsl")
            ->addTexture("test_res/textures/tex_cube.png", OGLR::Texture::Type::X4B);

    Terrain::initTerrain();
    terrain = Terrain::buildTile(tSettingsNew);

}

void TestLayer::onRender() {
    ImGui::Begin("Renderer settings");

        ImGui::Text("Average render time (ms): %f", avrgFrameTime);
        ImGui::Checkbox("Render cube", &renderCube);

        ImGui::SliderInt("Mesh resolution", (int*)&tSettingsNew.resolution, 0, 512);
        ImGui::SliderInt("Number of iterations", (int*)&tSettingsNew.iter, 0, 128);
        ImGui::SliderAngle("Terrain offset angle", &tSettingsNew.angle, 0, 2 * 360);

    ImGui::End();

    bool newSettings = !(tSettingsNew == tSettingsOld);
    tSettingsOld = tSettingsNew;



    m_Renderer.setCamera(m_Camera);
    if (renderCube)
        m_Renderer.render(mesh, glm::mat4(1.0f));

    m_Renderer.render(terrain, glm::mat4(1.0f));

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
    Terrain::destroyTerrain();
    delete mesh;
}
