#pragma once

#include "OGLR.h"

class ParticleLayer: public OGLR::Layer {
public:

    void onAttach() override;
    void onRender() override;
    void onUpdate(float dt) override;
    void onDetach() override;


private:

    struct ParticleSettings {
        glm::vec4 camPos;
        glm::ivec2 viewport_size;
        float pointSize = 3e-3f;
        float nearPlane = 1e-4f;
    } m_pSettings;

    OGLR::Renderer m_Renderer {};
    OGLR::Camera m_Camera {1.0f * glm::vec3{0.0f, 1.0f, -1.0f},  {0.0f, -1.0f, 1.0f}};

    OGLR::Buffer* particles;
    OGLR::Buffer* ubo;
    OGLR::VertexArray va;
    OGLR::Shader* render_shader;
    OGLR::Shader* compute_shader;

    GLuint resolution = 10;
    GLuint nb_particles = resolution * resolution * resolution;
    float time = 0;

    using PointVertex = OGLR::Vertex<glm::vec4, glm::vec4, glm::vec4>;

};


