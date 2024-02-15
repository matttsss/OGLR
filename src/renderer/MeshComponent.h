#pragma once

#include "../buffers/Buffer.h"
#include "../buffers/VertexArray.h"

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

#include <vector>

namespace OGLR
{

	struct MeshComponent
	{

		MeshComponent() = delete;
        MeshComponent(Buffer&& vb, Buffer&& ib, VertexArray&& va) noexcept;

        template <typename ... VT>
        MeshComponent(const std::vector<Vertex<VT...>>& vertices, const std::vector<uint32_t>& indices)
            : va(), vb(BufType::VBO, vertices.data(), vertices.size() * sizeof(Vertex<VT...>)),
                    ib(BufType::IBO, indices.data(), indices.size() * sizeof(GLuint))
        {

            va.bind();
            vb.bind();

            va.bindAttributes<Vertex<VT...>>();

            VertexArray::unBind();
            vb.unBind();
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


		VertexArray va;
        Buffer ib;
        Buffer vb;

		Shader* shader = nullptr;
		std::vector<Texture> textures;

	};

}


