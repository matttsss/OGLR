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
        static constexpr short ATTRIBUTES_SIZE = 11;

        Vertex(glm::vec3 position = glm::vec3(0.0f),
               glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f),
               glm::vec3 color = glm::vec3{1.0f},
               glm::vec2 uv = glm::vec3{0.0f}):
            position(position), normal(normal), color(color), uv(uv) {}


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
        MeshComponent(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		~MeshComponent();

        MeshComponent* addTexture(Texture texture);
        MeshComponent* addShader(const std::string& vertPath, const std::string& fragPath);

        void bind() const;
        void unBind() const;

        static MeshComponent* loadFromObjFile(const std::string &objPath);


		Buffers::VertexArray va;
		Buffers::VertexBuffer vb;
		Buffers::IndexBuffer ib;

		Shader* shader = nullptr;
		std::vector<Texture> textures;

	};

}


