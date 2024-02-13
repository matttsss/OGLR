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
        mesh->shader->setUniformMat4f("u_MVP", m_PVMatrix * modelTransform);

		glDrawElements(GL_TRIANGLES, mesh->ib.getCount(), GL_UNSIGNED_INT, nullptr);
        mesh->unBind();
    }

    void Renderer::render(const Terrain &terrain) const {
        Shader* shader = terrain.renderShader;
        int32_t radius = (int32_t)terrain.settings.radius;
        TerrainBuffers& tb = Terrain::getBuffersForRes(terrain.settings.resolution);

        shader->bind();
        tb.va.bind();
        tb.ib.bind();

        shader->setUniform1i("u_NHMap", 0);

        for (int32_t i = -radius; i <= radius; ++i) {
            for (int32_t j = -radius; j <= radius; ++j) {
                const Texture& texture = terrain.getNHTextureAtPos({i, j});
                texture.bind();
                shader->setUniformMat4f("u_MVP", glm::translate(m_PVMatrix, {i, 0, j}));
                glDrawElements(GL_TRIANGLES, tb.ib.getCount(), GL_UNSIGNED_INT, nullptr);
            }
        }

        Texture::unBind();
        Shader::unBind();
        VertexArray::unBind();
        Buffer<BufferType::Idx>::unBind();
    }

}