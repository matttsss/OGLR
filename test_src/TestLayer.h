#include "OGLR.h"

class TestLayer : public OGLR::Layer
{

public:

    ~TestLayer();

    void onAttach() override;
    void onRender() override;
    void onUpdate(float dt) override;
    void onDettach() override;


private:

    OGLR::Renderer m_Renderer {};
    OGLR::Camera m_Camera {};

    OGLR::MeshComponent* mesh = nullptr;

    float vertices[16] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f
    };

    GLuint indices[6] = {
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
    };


};
