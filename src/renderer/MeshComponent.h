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
        static const short ATTRIBUTES_SIZE = 11;

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

        const float* toArray() const;

    };
	

	struct MeshComponent
	{

		MeshComponent() = delete;
        MeshComponent(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		~MeshComponent();

        MeshComponent* addTexture(const std::string& texPath);
        MeshComponent* addShader(const std::string& vertPath, const std::string& fragPath);

        static MeshComponent* loadFromObjFile(const std::string &objPath);

		Buffers::VertexArray va;
		Buffers::VertexBuffer vb;
		Buffers::IndexBuffer ib;

		Shader* shader = nullptr;
		Texture* texture = nullptr;

    private:
        static float* flattenVertices(const std::vector<Vertex> &vertices);

	};

}


