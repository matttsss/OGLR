#include "Renderer.h"

#include "../app/Application.h"
#include "../utils/debug.h"

namespace OGLR
{

    std::vector<uint32_t> Renderer::s_Indices {
            0, 1, 2,
            2, 3, 0
    };

    std::vector<Vertex<glm::vec3>> Renderer::s_BackGroundVert {
            Vertex<glm::vec3>(glm::vec3(-1, -1, 0.99999)),
            Vertex<glm::vec3>(glm::vec3(-1,  1, 0.99999)),
            Vertex<glm::vec3>(glm::vec3( 1,  1, 0.99999)),
            Vertex<glm::vec3>(glm::vec3( 1, -1, 0.99999)),
    };

	Renderer::Renderer()
        : m_BeckGroundUBO(BufType::UBO, nullptr, 3 * sizeof(glm::vec4), UsageType::Dynamic),
         m_BackGround(s_BackGroundVert, s_Indices)
	{

//#ifdef _DEBUG
		std::cout << "Setting debug callback" << std::endl;
		DEBUG::setDebugPriorityLevel(DEBUG::PriorityLevel::LOW);
		DEBUG::enableGLDebug();
//#endif

        m_BackGround.addShader("test_res/shaders/background.vert.glsl", "test_res/shaders/background.frag.glsl");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);


        // TODO sort culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

    }


	void Renderer::render(const MeshComponent* mesh, const glm::mat4& modelTransform) const
	{
		mesh->bind();
        mesh->shader->setUniform("u_MVP", m_PVMatrix * modelTransform);

		glDrawElements(GL_TRIANGLES, mesh->ib.getCount(), GL_UNSIGNED_INT, nullptr);
        mesh->unBind();
    }

    void Renderer::render(Terrain &terrain) const {
        Shader* shader = terrain.chunkRenderer;
        shader->bind();

        shader->setUniform("u_MVP", m_PVMatrix);

        for (const auto& [offSet, tb] : terrain.getChunks()) {
            tb.va.bind();
            shader->setUniform("u_Offset", offSet);
            glDrawElements(GL_TRIANGLES, tb.ib.getCount(), GL_UNSIGNED_INT, nullptr);
        }

        VertexArray::unBind();
        Shader::unBind();
    }

    void Renderer::beginFrame(Camera& camera) {
        p_Camera = &camera;
        m_PVMatrix = camera.getProjection() * camera.getView();
    }

    void Renderer::endFrame() {

        // Set UBO values
        glm::ivec2 frameBuffSize = Application::getFrameBufferSize();
        m_BGSettings.frameBuffWidth = frameBuffSize.x;
        m_BGSettings.frameBuffHeight = frameBuffSize.y;
        m_BGSettings.aim = p_Camera->getAim();
        m_BGSettings.uBase = glm::normalize(glm::cross(m_BGSettings.aim, Camera::s_UP));
        m_BGSettings.vBase = glm::normalize(glm::cross(m_BGSettings.uBase, m_BGSettings.aim));

        m_BeckGroundUBO.bind();
        m_BeckGroundUBO.setData(&m_BGSettings, sizeof(BGSettings));

        m_BackGround.shader->bind();
        m_BackGround.shader->setUniformBlock("AimSettings", m_BeckGroundUBO);

        m_BackGround.va.bind();
        m_BackGround.ib.bind();

        glDrawElements(GL_TRIANGLES, m_BackGround.ib.getCount(), GL_UNSIGNED_INT, nullptr);

        VertexArray::unBind();
        m_BackGround.ib.unBind();
        p_Camera = nullptr;
    }

}