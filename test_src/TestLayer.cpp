#include "TestLayer.h"

#include <glm/gtc/type_ptr.hpp>

#include "utils/Terrain.h"

void TestLayer::onAttach()
{
    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.1f, 50.f);

    mesh = OGLR::MeshComponent::loadFromObjFile("test_res/models/tex_cube.obj")
            ->addShader("test_res/shaders/textured_simple.vert.glsl", "test_res/shaders/textured_simple.frag.glsl")
            ->addTexture("test_res/textures/tex_cube.png", OGLR::Texture::Type::X4B);

    terrain.chunkRenderer = OGLR::Shader::fromGLSLTextFiles("test_res/shaders/terrain_chunk.vert.glsl", "test_res/shaders/terrain_chunk.frag.glsl");

}

void TestLayer::onRender() {
    ImGui::Begin("Renderer settings");

        ImGui::Text("Average render time (ms): %f", avrgFrameTime);
        ImGui::Checkbox("Render cube", &renderCube);
        if (renderCube)
        {
            ImGui::SliderFloat3("Cube position", &transform[3].x, -5, 5);
        }

        ImGui::DragFloat2("Tile pos", (float*)&cSettings.worldOffset);
        ImGui::DragInt2("Tile scale", (int*)&cSettings.scale, 1, 0);
        ImGui::InputInt("Mesh resolution", (int*)&cSettings.resolution);

        ImGui::SliderInt("Number of octaves", (int*)&tSeed.octaves, 0, 15);
        ImGui::SliderFloat("Terrain offset angle", &tSeed.angle, 0, 2*glm::pi<float>());
        ImGui::SliderFloat("Terrain frequency", &tSeed.frequ, 1, 20);
        ImGui::SliderFloat("Terrain height scale", &tSeed.zScale, 0.1, 5);

    ImGui::End();

    m_Renderer.beginFrame(m_Camera);

    if (renderCube)
        m_Renderer.render(mesh, transform);

    terrain.updateSettings(cSettings, tSeed);
    m_Renderer.render(terrain);

    m_Renderer.endFrame();

    //ImGui::ShowDemoWindow(nullptr);
}

void TestLayer::onUpdate(float dt)
{

    m_Camera.onUpdate(dt);
    terrain.onUpdate(m_Camera.getPosition());

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
