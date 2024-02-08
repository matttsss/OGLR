#pragma once

#include "../buffers/VertexArray.h"
#include "../buffers/VertexBufferLayout.h"
#include "MeshComponent.h"
#include "Shader.h"
#include "../utils/Camera.h"

namespace OGLR
{

	class Renderer
	{
	public:

		Renderer();

		void setCamera(Camera& camera);

		void render(const MeshComponent* mesh, const glm::mat4& modelTransform);

	private:

		glm::mat4 m_PVMatrix;

	};

}



