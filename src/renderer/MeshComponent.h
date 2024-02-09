#pragma once

#include "../buffers/Buffer.h"
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
        static MeshComponent* loadFromObjFile(const std::string &objPath);

		~MeshComponent();

        template <typename... TextArgs>
        MeshComponent* addTexture(TextArgs... texArgs) {
            textures.emplace_back(std::forward<TextArgs>(texArgs)...);
            return this;
        }
        MeshComponent* addShader(const std::string& vertPath, const std::string& fragPath);

        void bind() const;
        void unBind() const;


		Buffers::VertexArray va;
        Buffers::Buffer<Buffers::BufferType::Vertex> vb;
        Buffers::Buffer<Buffers::BufferType::Index> ib;

		Shader* shader = nullptr;
		std::vector<Texture> textures;

	};

}


