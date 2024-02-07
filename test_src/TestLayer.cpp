#include "TestLayer.h"

#include <glm/gtc/type_ptr.hpp>

#include "terrain/Terrain.h"

void TestLayer::onAttach()
{

    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.1f, 10.f);

    mesh = OGLR::MeshComponent::loadFromObjFile("test_res/models/tex_cube.obj")
            ->addShader("test_res/shaders/textured_simple.vert.glsl", "test_res/shaders/textured_simple.frag.glsl")
            ->addTexture(OGLR::Texture{"test_res/textures/tex_cube.png", OGLR::Texture::Type::X4B});

    Terrain::initTerrain();
    terrain = Terrain::buildTile(1024, 1);

}

void TestLayer::onRender() {
    static bool renderCube = false;

    ImGui::Begin("Renderer settings");

        ImGui::Text("Average render time (ms): %f", avrgFPS);
        ImGui::Checkbox("Render cube", &renderCube);

    ImGui::End();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO find somewhere better


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
        avrgFPS = 0;
        for (float t : renderTimes)
            avrgFPS += t;
        avrgFPS /= 100;
        //avrgFPS = 1.0f/avrgFPS * 1e3;

    }

}

void TestLayer::onDettach()
{
    Terrain::destroyTerrain();
    delete mesh;
}
