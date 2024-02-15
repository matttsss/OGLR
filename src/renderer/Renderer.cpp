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
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);

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

    void Renderer::render(const Terrain &terrain) const {
        Shader* shader = terrain.renderShader;
        uint32_t radius = 5;
        const TerrainBuffers& tb = Terrain::getBuffersForRes(terrain.cSettings.resolution);

        shader->bind();
        tb.va.bind();
        tb.ib.bind();


        //for (int32_t i = -radius; i <= radius; ++i) {
        //    for (int32_t j = -radius; j <= radius; ++j) {
        //        const Texture& texture = terrain.getNHTextureAtPos({i, j});
        //        texture.bind();
        //        shader->setUniform("u_MVP", glm::translate(m_PVMatrix, {i, 0, j}));
        //        glDrawElements(GL_TRIANGLES, tb.ib.getCount(), GL_UNSIGNED_INT, nullptr);
        //    }
        //}

        const Texture& texture = terrain.getNHTextureAtPos({0, 0});
        texture.bind();

        shader->setUniform("u_Radius", radius);
        shader->setUniform("u_NHMap", 0);
        shader->setUniform("u_MVP", m_PVMatrix);

        GLsizei nbInstances = (2*radius + 1) * (2*radius + 1);
        glDrawElementsInstanced(GL_TRIANGLES, tb.ib.getCount(), GL_UNSIGNED_INT, nullptr, nbInstances);


        Texture::unBind();
        Shader::unBind();
        VertexArray::unBind();
        tb.ib.unBind();
    }

}