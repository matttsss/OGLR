#pragma once

#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBufferLayout.h"

#include "Texture.h"
#include "Shader.h"

namespace OGLR
{

	struct MeshComponent
	{

		MeshComponent(const GLfloat* vertices, GLuint sizeOfVertices, const GLuint* indices, GLsizei countOfIndices,
			const Buffers::VertexBufferLayout& vbl,
			const std::string& vertPath, const std::string& fragPath, const std::string& texPath = "");

		~MeshComponent();

		Buffers::VertexArray va;
		Buffers::VertexBuffer vb;
		Buffers::IndexBuffer ib;

		Shader* shader;
		Texture* texture;

	};

}


