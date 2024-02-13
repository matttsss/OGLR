#pragma once

#include "../buffers/Buffer.h"
#include "../buffers/VertexArray.h"
#include "../buffers/VertexBufferLayout.h"

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

namespace OGLR
{

	struct MeshComponent
	{

		MeshComponent() = delete;

        template <typename ... VT>
        MeshComponent(const std::vector<Vertex<VT...>>& vertices, const std::vector<uint32_t>& indices)
            : va(), vb(vertices.data(), vertices.size() * sizeof(Vertex<VT...>)),
                  ib(indices.data(), indices.size() * sizeof(GLuint))
        {
            VertexBufferLayout vbl;
            (vbl.addAttr<VT>(), ...);

            va.bind();
            vb.bind();
            va.bindAttributes(vbl);

            VertexArray::unBind();
            Buffer<BufferType::Vtx>::unBind();

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
        Buffer<BufferType::Vtx> vb;
        Buffer<BufferType::Idx> ib;

		Shader* shader = nullptr;
		std::vector<Texture> textures;

	};

}


