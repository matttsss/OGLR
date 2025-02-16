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

    void compute_densities() const;
    void update_particles(float dt) const;

    OGLR::Renderer m_Renderer {};
    OGLR::Camera m_Camera {1.0f * glm::vec3{0.0f, 1.0f, -1.0f},  {0.0f, -1.0f, 1.0f}};

    OGLR::Buffer* render_ubo = nullptr;
    OGLR::Shader* render_shader = nullptr;
    OGLR::Buffer* sim_ubo = nullptr;
    OGLR::Shader* densities_shader = nullptr;
    OGLR::Shader* update_particles_shader = nullptr;

    uint8_t current_buffer = 0;
    OGLR::VertexArray va;
    OGLR::Buffer* densities = nullptr;
    OGLR::Buffer* particles[2] = {nullptr, nullptr};

    struct ParticleRenderSettings {
        glm::ivec2 viewport_size = glm::ivec2(0);
        float pointSize = 3e-3f;
    } m_render_settings;

    bool paused = true;
    struct ParticleSimSettings {
        GLuint nb_particles = 0;
        float viscosity_mul = 1.f;
        float pressure_mul = 1.f;
        float pressure_target = 2.75f;
        float kernel_radius = 0.1f;
    } m_sim_settings;

    static constexpr uint32_t NB_FRAMES = 100;
    float average_frame_time = 0.f;
    uint32_t frame_idx = 0;
    float frame_times[NB_FRAMES] = {};

};


