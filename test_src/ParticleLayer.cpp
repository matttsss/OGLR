#include "ParticleLayer.h"

void ParticleLayer::onAttach() {
    m_Camera.setSpeed(1e-3, 1e-3);
    m_Camera.setPerspectiveProjection(glm::radians(50.f), 1.6f, 0.0001f, 800.f);


    std::vector<PointVertex> vb_temp;

    glEnable(GL_PROGRAM_POINT_SIZE);

    for (uint32_t i = 0; i < 100; ++i) {
        for (uint32_t j = 0; j < 100; ++j) {
            vb_temp.emplace_back(glm::vec4(i / 100.f, 0.f, j / 100.f, 1.f), glm::vec4(1.f));
        }
    }

    vb = new OGLR::Buffer(OGLR::BufType::VBO, vb_temp.data(), vb_temp.size() * PointVertex::N);
    vb->bind();
    va.bind();

    va.bindAttributes<PointVertex>();

    vb->unBind();
    OGLR::VertexArray::unBind();

    shader = OGLR::Shader::fromGLSLTextFiles("../test_res/shaders/particles.vert.glsl", "../test_res/shaders/particles.frag.glsl");

}

void ParticleLayer::onRender() {


    m_Renderer.beginFrame(m_Camera);

    shader->bind();
    shader->setUniform("u_MVP", m_Camera.getProjection() * m_Camera.getView());

    vb->bind();
    va.bind();

    glDrawArrays(GL_POINTS, 0, 10000);

    OGLR::VertexArray::unBind();
    vb->unBind();
    OGLR::Shader::unBind();

    m_Renderer.endFrame();

}

void ParticleLayer::onUpdate(float dt) {
    m_Camera.onUpdate(dt);

}

void ParticleLayer::onDetach() {

    delete vb;
    delete shader;

}
