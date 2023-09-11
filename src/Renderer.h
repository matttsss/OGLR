#pragma once

#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBufferLayout.h"
#include "MeshComponent.h"
#include "Shader.h"


namespace OGLR
{

	class Renderer
	{
	public:

		Renderer();

		void clear() const;

		void render(const MeshComponent& mesh, const glm::mat4& transform);

	private:

	};

}



