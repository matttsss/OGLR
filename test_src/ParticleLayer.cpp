#include "ParticleLayer.h"

void ParticleLayer::onAttach() {
    m_Camera.setSpeed(1e-3, 1e-3);
    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 1e-4f, 800.f);
    m_pSettings.viewport_size = OGLR::Application::getFrameBufferSize();

    glEnable(GL_PROGRAM_POINT_SIZE);
    ubo = new OGLR::Buffer(OGLR::BufType::UBO, &m_pSettings, sizeof(ParticleSettings), OGLR::UsageType::Dynamic);

    const std::vector<PointVertex> vb_temp = spawn_cube(50, {0, 0, 0});

    {   // Initialize first buffer
        particles[0] = new OGLR::Buffer(OGLR::BufType::VBO, vb_temp.data(), vb_temp.size() * PointVertex::N);
    }

    {   // Initialize second buffer and vertex array
        particles[1] = new OGLR::Buffer(OGLR::BufType::VBO, vb_temp.data(), vb_temp.size() * PointVertex::N);
        particles[1]->bind();
        va.bind();

        va.bindAttributes<PointVertex>();

        particles[1]->unBind();
        OGLR::VertexArray::unBind();
    }

    std::cout << "nb_particles: " << nb_particles << std::endl;
    densities = new OGLR::Buffer(OGLR::BufType::SSBO, nullptr, sizeof(GLfloat) * nb_particles, OGLR::UsageType::Dynamic);

    update_particles_shader = OGLR::Shader::fromGLSLTextFiles("../test_res/shaders/particle_shaders/particles.glsl");
    densities_shader = OGLR::Shader::fromGLSLTextFiles("../test_res/shaders/particle_shaders/compute_particle_densities.glsl");
    render_shader = OGLR::Shader::fromGLSLTextFiles("../test_res/shaders/particle_shaders/particles.vert.glsl",
                                                    "../test_res/shaders/particle_shaders/particles.frag.glsl");
}

void ParticleLayer::onRender() {
    ImGui::Begin("Particle settings");
        ImGui::SliderFloat("Pressure multiplier", &pressure_mul, 1e-3f, 0.5f);
        ImGui::SliderFloat("Target pressure", &pressure_target, 0.0f, 1.0f);
        ImGui::SliderFloat("Point size", &m_pSettings.pointSize, 1e-3f, 0.2f);
        ImGui::SliderFloat("Kernel radius", &kernel_radius, 1e-3f, 2.f);
        ImGui::Checkbox("Paused", &paused);
    ImGui::End();

    m_Renderer.beginFrame(m_Camera);

    ubo->bind();
    ubo->setData(&m_pSettings, sizeof(ParticleSettings));

    render_shader->bind();
    render_shader->setUniformBlock("u_ParticleSettings", *ubo);
    render_shader->setUniform("u_proj", m_Camera.getProjection());
    render_shader->setUniform("u_view", m_Camera.getView());

    particles[current_buffer]->bind();
    va.bind();

    glDrawArrays(GL_POINTS, 0, nb_particles);

    OGLR::VertexArray::unBind();
    particles[current_buffer]->unBind();
    OGLR::Shader::unBind();

    m_Renderer.endFrame();

}

void ParticleLayer::onUpdate(float dt) {
    particles[0]->castTo(OGLR::BufType::SSBO, OGLR::UsageType::Dynamic);
    particles[1]->castTo(OGLR::BufType::SSBO, OGLR::UsageType::Dynamic);

    if (!paused) {
        compute_densities();
        update_particles(dt);
    }

    m_Camera.onUpdate(dt);
    current_buffer = (current_buffer + 1) % 2;

    particles[0]->castTo(OGLR::BufType::VBO, OGLR::UsageType::Dynamic);
    particles[1]->castTo(OGLR::BufType::VBO, OGLR::UsageType::Dynamic);
}

void ParticleLayer::onDetach() {
    delete particles[0];
    delete particles[1];
    delete ubo;
    delete render_shader;
    delete densities_shader;
    delete update_particles_shader;
}


void ParticleLayer::compute_densities() const {
    densities_shader->bind();
    densities_shader->setUniform<GLfloat>("u_radius", kernel_radius);
    densities_shader->setUniform<GLuint>("u_nb_particles", nb_particles);

    particles[current_buffer]->bindAsBufferBase(0);
    densities->bindAsBufferBase(1);

    glDispatchCompute(nb_particles, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    OGLR::Shader::unBind();
    densities->unBind();
    particles[current_buffer]->unBind();
}

void ParticleLayer::update_particles(float dt) const {
    update_particles_shader->bind();
    update_particles_shader->setUniform<GLfloat>("u_dt", dt * 1e-3f);
    update_particles_shader->setUniform<GLuint>("u_nb_particles", nb_particles);
    update_particles_shader->setUniform<GLfloat>("u_radius", kernel_radius);
    update_particles_shader->setUniform<GLfloat>("u_pressure_multiplier", pressure_mul);
    update_particles_shader->setUniform<GLfloat>("u_pressure_target", pressure_target);

    uint8_t next_buffer = (current_buffer + 1) % 2;

    particles[current_buffer]->bindAsBufferBase(0);
    particles[next_buffer]->bindAsBufferBase(1);
    densities->bindAsBufferBase(2);

    glDispatchCompute(nb_particles, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    OGLR::Shader::unBind();
    particles[current_buffer]->unBind();
    particles[next_buffer]->unBind();
}


std::vector<ParticleLayer::PointVertex> ParticleLayer::spawn_cube(GLuint resolution, const glm::vec3& center) {
    std::vector<PointVertex> vb_temp;

    for (uint32_t i = 0; i < resolution; ++i) {
        for (uint32_t j = 0; j < resolution; ++j) {
            //for (uint32_t k = 0; k < 1; ++k) {
                glm::vec4 spawn_pos = glm::vec4((float)i / resolution , (float)0 / resolution, (float)j / resolution, 1.f);
                          spawn_pos += glm::vec4(center, 0.f);
                          spawn_pos -= glm::vec4(0.5f, 0.5f, 0.5f, 0.f);

                vb_temp.emplace_back(spawn_pos, glm::vec4(0.f));
            //}
        }
    }

    vb_temp.emplace_back(glm::vec4(center, 1.f), glm::vec4(0.f));
    nb_particles = vb_temp.size();

    return vb_temp;
}

std::vector<ParticleLayer::PointVertex> ParticleLayer::spawn_disk(GLuint resolution, const glm::vec3& center) {
    std::vector<PointVertex> vb_temp;

    for (uint32_t i = 0; i < resolution; ++i) {
        for (uint32_t j = 0; j < resolution; ++j) {
            glm::vec2 spawn_pos = glm::vec2((float)i / resolution , (float)j / resolution);
            spawn_pos *= glm::vec2{3, glm::two_pi<float>()};
            spawn_pos  = glm::vec2{glm::cos(spawn_pos.y), glm::sin(spawn_pos.y)} * spawn_pos.x;
            spawn_pos += glm::vec2(center.x, center.z);
            spawn_pos -= glm::vec2(0.5f);

            vb_temp.emplace_back(glm::vec4(spawn_pos.x, center.y - 0.5f, spawn_pos.y, 1.f), glm::vec4(0.f));
        }
    }

    nb_particles = vb_temp.size();
    return vb_temp;
}
