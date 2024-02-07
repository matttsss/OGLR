#include "OGLR.h"

class TestLayer : public OGLR::Layer
{

public:

    void onAttach() override;
    void onRender() override;
    void onUpdate(float dt) override;
    void onDettach() override;


private:

    OGLR::Renderer m_Renderer {};
    OGLR::Camera m_Camera {{0.0f, -1.0f, -1.0f},  {0.0f, 1.0f, 1.0f}};

    OGLR::MeshComponent* mesh = nullptr;
    OGLR::MeshComponent* terrain = nullptr;

    float renderTimes[100];
    char16_t renderTimeIdx = 0;
    float avrgFPS = 0;

};
