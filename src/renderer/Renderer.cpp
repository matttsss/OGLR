#include "Renderer.h"

#include "../utils/debug.h"

namespace OGLR
{

	Renderer::Renderer()
	{

//#ifdef _DEBUG
		std::cout << "Setting debug callback" << std::endl;
		DEBUG::setDebugPriorityLevel(DEBUG::PriorityLevel::LOW);
		DEBUG::enableGLDebug();
//#endif
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);


        // TODO sort culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

    }


	void Renderer::setCamera(Camera& camera)
	{
		m_PVMatrix = camera.getProjection() * camera.getView();
	}

	void Renderer::render(const MeshComponent* mesh, const glm::mat4& modelTransform) const
	{
		mesh->bind();
        mesh->shader->setUniform("u_MVP", m_PVMatrix * modelTransform);

		glDrawElements(GL_TRIANGLES, mesh->ib.getCount(), GL_UNSIGNED_INT, nullptr);
        mesh->unBind();
    }

    void Renderer::render(Terrain &terrain) const {
        MeshComponent& mesh = terrain.getChunk({0, 0});
        mesh.shader = terrain.chunkRenderer;
        render(&mesh, glm::mat4(1));
        mesh.shader = nullptr;
    }

}