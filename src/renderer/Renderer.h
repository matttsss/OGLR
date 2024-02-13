#pragma once

#include "MeshComponent.h"
#include "../utils/Camera.h"
#include "../utils/Terrain.h"

namespace OGLR
{
	class Renderer
	{
	public:

		Renderer();

		void setCamera(Camera& camera);

		void render(const MeshComponent* mesh, const glm::mat4& modelTransform) const;
        void render(const Terrain& terrain) const;

	private:

		glm::mat4 m_PVMatrix;

	};

}



