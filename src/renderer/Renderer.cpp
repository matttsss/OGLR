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
        Shader* shader = terrain.getShader();
        TerrainBuffers& tb = Terrain::getBuffersForRes(terrain.settings.resolution);
        const auto& textures = terrain.getTextures();

        shader->bind();
        tb.va.bind();
        tb.ib.bind();

        textures[0].bind();
        shader->setUniform1i("u_Texture0", 0);

        shader->setUniformMat4f("u_MVP", m_PVMatrix);
        glDrawElements(GL_TRIANGLES, tb.ib.getCount(), GL_UNSIGNED_INT, nullptr);

        Texture::unBind();
        Shader::unBind();
        VertexArray::unBind();
        Buffer<BufferType::Idx>::unBind();

    }

}