#pragma once

#include "../buffers/VertexBuffer.h"
#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexArray.h"
#include "../buffers/VertexBufferLayout.h"

#include "Texture.h"
#include "Shader.h"

namespace OGLR
{

    struct Vertex
    {
        glm::vec3 position { 0.0f };
        glm::vec3 normal   { 0.0f };
        glm::vec3 color    { 1.0f };
        glm::vec2 uv       { 0.0f };

        bool operator==(const Vertex& other) const
        {
            return
            position == other.position &&
            normal == other.normal &&
            color == other.color &&
            uv == other.uv;
        }

    };
	

	struct MeshComponent
	{

		MeshComponent() = delete;
        MeshComponent(const std::string &objPath, const std::string& shaderPathExtentionless);

		~MeshComponent();

		Buffers::VertexArray va;
		Buffers::VertexBuffer vb;
		Buffers::IndexBuffer ib;

		Shader* shader;
		Texture* texture;

	};

}


