#pragma once

#include "../buffers/VertexBuffer.h"
#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexArray.h"
#include "../buffers/VertexBufferLayout.h"

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
        MeshComponent(const std::string& objPath);
        MeshComponent(const std::string& objPath, const std::string& shaderName);

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


