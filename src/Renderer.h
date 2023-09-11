#pragma once

#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBufferLayout.h"
#include "MeshComponent.h"
#include "Shader.h"

using namespace OGLR::Buffers;

namespace OGLR
{

	class Renderer
	{
	public:

		Renderer();

		void clear() const;

		void render(const MeshComponent& mesh);

	private:

	};

}



