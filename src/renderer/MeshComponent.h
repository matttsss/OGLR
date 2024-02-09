#pragma once

#include "../buffers/Buffer.h"
#include "../buffers/VertexArray.h"
#include "../buffers/VertexBufferLayout.h"

#include "Texture.h"
#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

#include <cstring>

namespace OGLR
{

    template <typename ... VTs>
    struct Vertex
    {
        static constexpr uint32_t N = (sizeof(VTs) + ...);

        char data[N];

        Vertex() = default;
        Vertex(const VTs& ... args) {

            uint32_t idx = 0;
            auto cpyMember = [&](const auto& val) {
                memcpy(data + idx, &val, sizeof(val));
                idx += sizeof(val);
            };

            (cpyMember(args), ...);
        }

        bool operator==(const Vertex<VTs...>& other) const {
            return !memcmp(data, other.data, N);
        }

    };
	

	struct MeshComponent
	{

		MeshComponent() = delete;

        template <typename ... VT>
        MeshComponent(const std::vector<Vertex<VT...>>& vertices, const std::vector<uint32_t>& indices)
            : va(), vb(vertices.data(), vertices.size() * sizeof(Vertex<VT...>)),
                  ib(indices.data(), indices.size() * sizeof(GLuint))
        {
            Buffers::VertexBufferLayout vbl;
            (vbl.addAttr<VT>(), ...);

            vb.bind();
            ib.bind();

            va.bind();
            va.bindAttributes(vb, vbl);
        }


        static MeshComponent* loadFromObjFile(const std::string &objPath);

		~MeshComponent();

        template <typename... TextArgs>
        MeshComponent* addTexture(TextArgs&&... texArgs) {
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


