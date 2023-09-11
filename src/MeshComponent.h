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

		struct MeshComponentBuilder
		{

			void setVertices(GLfloat* vertices, GLuint verticesSize);
			void setVerticesLayout(Buffers::VertexBufferLayout& vbl);
			void setIndices(GLuint* indices, GLsizei indicesCount);
			void setShaderPath(const std::string& vertPath, const std::string& fragPath);
			void setTexturePath(const std::string& texPath);
			void setColor(const glm::vec4& color);

			MeshComponent* build();

		private:

			GLfloat* vertices;
			GLuint verticesSize;
			GLuint* indices;
			GLsizei indicesCount;
			glm::vec4 color;

			Buffers::VertexBufferLayout vbl;
			std::string vertPath, fragPath, texturePath;

		};

		MeshComponent() = delete;
		~MeshComponent();

		Buffers::VertexArray* va;
		Buffers::VertexBuffer* vb;
		Buffers::IndexBuffer* ib;

		Shader* shader;
		Texture* texture;

	private:

		MeshComponent(Buffers::VertexArray* va, Buffers::VertexBuffer* vb, Buffers::IndexBuffer* ib,
					  Shader* shader, Texture* texture);

	};

}


