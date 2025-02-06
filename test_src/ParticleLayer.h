#pragma once

#include "OGLR.h"

class ParticleLayer: public OGLR::Layer {
public:

    void onAttach() override;
    void onRender() override;
    void onUpdate(float dt) override;
    void onDetach() override;


private:
    using PointVertex = OGLR::Vertex<glm::vec4, glm::vec4>;

    std::vector<PointVertex> spawn_cube(GLuint resolution, const glm::vec3& center = {0, 0, 0});
    std::vector<PointVertex> spawn_disk(GLuint resolution, const glm::vec3& center = {0, 0, 0});

    void compute_densities();
    void update_particles(float dt);

    struct ParticleSettings {
        glm::ivec2 viewport_size;
        float pointSize = 3e-3f;
    } m_pSettings;

    OGLR::Renderer m_Renderer {};
    OGLR::Camera m_Camera {1.0f * glm::vec3{0.0f, 1.0f, -1.0f},  {0.0f, -1.0f, 1.0f}};

    OGLR::Buffer* ubo;
    OGLR::Shader* render_shader;
    OGLR::Shader* densities_shader;
    OGLR::Shader* update_particles_shader;

    uint8_t current_buffer = 0;
    OGLR::VertexArray va;
    OGLR::Buffer* densities;
    OGLR::Buffer* particles[2];

    float kernel_radius = 0.5f;
    GLuint nb_particles = 0;
    bool paused = true;

};


