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
    OGLR::Camera m_Camera {{0.0f, 0.0f, 0.0f},  {0.0f, 1.0f, 0.0f}};

    OGLR::MeshComponent* mesh = nullptr;

    float vertices[20] = {
            -0.5f, 0.5f, -0.5f,   /* UV */0.0f, 0.0f,
            0.5f, 0.5f, -0.5f,    /* UV */1.0f, 0.0f,
            0.5f, 0.5f,  0.5f,  /* UV */1.0f, 1.0f,
            -0.5f, 0.5f,  0.5f, /* UV */0.0f, 1.0f
    };

    GLuint indices[6] = {
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
    };


};
