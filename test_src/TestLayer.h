#include "OGLR.h"

#include "utils/Terrain.h"

class TestLayer : public OGLR::Layer
{

public:

    void onAttach() override;
    void onRender() override;
    void onUpdate(float dt) override;
    void onDetach() override;


private:

    OGLR::Renderer m_Renderer {};
    OGLR::Camera m_Camera {2.0f * glm::vec3{0.0f, 1.0f, -1.0f},  {0.0f, -1.0f, 1.0f}};

    OGLR::MeshComponent* mesh = nullptr;
    OGLR::MeshComponent* terrain = nullptr;

    TerrainSettings tSettingsOld;
    TerrainSettings tSettingsNew;
    bool renderCube = false;

    float renderTimes[100];
    char16_t renderTimeIdx = 0;
    float avrgFrameTime = 0;

};
