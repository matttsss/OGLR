#pragma once

#include "OGLR.h"

class ParticleLayer: public OGLR::Layer {
public:

    void onAttach() override;
    void onRender() override;
    void onUpdate(float dt) override;
    void onDetach() override;


private:

    OGLR::Renderer m_Renderer {};
    OGLR::Camera m_Camera {0.0f * glm::vec3{0.0f, 1.0f, -1.0f},  {0.0f, -1.0f, 1.0f}};

    OGLR::Buffer* vb;
    OGLR::VertexArray va;
    OGLR::Shader* shader;

    float renderTimes[100];
    char16_t renderTimeIdx = 0;
    float avrgFrameTime = 0;

    using PointVertex = OGLR::Vertex<glm::vec4, glm::vec4>;

};


