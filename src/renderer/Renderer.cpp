#include "Renderer.h"

#include "../utils/debug.h"

namespace OGLR
{

	Renderer::Renderer()
	{

#ifdef _DEBUG 
		std::cout << "Setting debug callback" << std::endl;
		DEBUG::setDebugPriorityLevel(DEBUG::PriorityLevel::LOW);
		DEBUG::enableGLDebug();
#endif
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}


	void Renderer::setCamera(Camera& camera)
	{
		m_PVMatrix = camera.getProjection() * camera.getView();
	}

	void Renderer::render(const MeshComponent* mesh, const glm::mat4& modelTransform)
	{
		mesh->shader->bind();
		mesh->va->bind();
		mesh->ib->bind();

		if (mesh->texture)
			mesh->texture->bind();

		mesh->shader->setUniformMat4f("u_MVP", m_PVMatrix * modelTransform);

		glDrawElements(GL_TRIANGLES, mesh->ib->getCount(), GL_UNSIGNED_INT, nullptr);
	}

}