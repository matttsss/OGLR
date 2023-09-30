#include "MeshComponent.h"

#include "../utils/utils.h"

#include <vector>
#include <unordered_map>
#include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

using namespace OGLR::Buffers;

namespace std
{
    template<>
    struct __attribute__((unused)) hash<OGLR::Vertex>
    {
        size_t operator()(OGLR::Vertex const &vertex) const
        {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.normal, vertex.color, vertex.uv);
            return seed;
        }
    };

}



namespace OGLR
{

    static tinyobj::ObjReader reader;
    static tinyobj::ObjReaderConfig reader_config;

	MeshComponent::MeshComponent(VertexArray* va, VertexBuffer* vb, IndexBuffer* ib,
		Shader* shader, Texture* texture)
		: va(va), vb(vb), ib(ib), shader(shader), texture(texture)
	{
	}

    MeshComponent::MeshComponent(const std::string &objPath)
    {
        if (!reader.ParseFromFile(objPath, reader_config))
        {
            if (!reader.Error().empty())
                std::cerr << "TinyObjReader: " << reader.Error();
            exit(1);
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::unordered_map<Vertex, uint32_t> uniqueVertices {};

        // Loop over shapes
        for (const auto & shape : shapes)
        {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
            {
                Vertex vert {};
                size_t fv = size_t(shape.mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                    vert.position = {
                            attrib.vertices[3 * idx.vertex_index + 0],
                            attrib.vertices[3 * idx.vertex_index + 1],
                            attrib.vertices[3 * idx.vertex_index + 2]
                    };

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0) {
                        vert.normal = {
                                attrib.normals[3 * idx.normal_index + 0],
                                attrib.normals[3 * idx.normal_index + 1],
                                attrib.normals[3 * idx.normal_index + 2]
                        };
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0) {
                        vert.uv = {
                                attrib.texcoords[2 * idx.texcoord_index + 0],
                                attrib.texcoords[2 * idx.texcoord_index + 1]
                        };
                    }

                    auto colorIndex = 3 * idx.vertex_index + 2;
                    if (colorIndex < attrib.colors.size())
                    {
                        vert.color = {
                                attrib.colors[colorIndex - 2],
                                attrib.colors[colorIndex - 1],
                                attrib.colors[colorIndex - 0],
                        };
                    }
                    else
                    {
                        vert.color = { 1.0f, 1.0f, 1.0f };
                    }


                }
                index_offset += fv;

                // per-face material
                shape.mesh.material_ids[f];

                if (uniqueVertices.count(vert) == 0)
                {
                    uniqueVertices[vert] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vert);
                }

                indices.push_back(uniqueVertices[vert]);

            }
        }


    }



    MeshComponent::~MeshComponent()
	{
		delete va;
		delete vb;
		delete ib;
		delete texture;
		delete shader;
	}

    void MeshComponent::MeshComponentBuilder::setVertices(GLfloat* vertices, GLuint verticesSize)
	{
		this->vertices = vertices;
		this->verticesSize = verticesSize;
	}

	void MeshComponent::MeshComponentBuilder::setVerticesLayout(VertexBufferLayout& vbl)
	{
		this->vbl = vbl;
	}

	void MeshComponent::MeshComponentBuilder::setIndices(GLuint* indices, GLsizei indicesCount)
	{
		this->indices = indices;
		this->indicesCount = indicesCount;
	}

	void MeshComponent::MeshComponentBuilder::setShaderPath(const std::string& vertPath, const std::string& fragPath)
	{
		this->vertPath = vertPath;
		this->fragPath = fragPath;
	}

	void MeshComponent::MeshComponentBuilder::setTexturePath(const std::string& texPath)
	{
		this->texturePath = texPath;
	}

	void MeshComponent::MeshComponentBuilder::setColor(const glm::vec4& color)
	{
		this->color = color;
	}

	MeshComponent* MeshComponent::MeshComponentBuilder::build()
	{

        VertexArray* pVertexArray = new VertexArray();
        VertexBuffer* pVertexBuffer = new VertexBuffer(vertices, verticesSize);
        IndexBuffer* pIndexBuffer = new IndexBuffer(indices, indicesCount);


		auto* shader = Shader::FromGLSLTextFiles(vertPath, fragPath);
		Texture* texture = nullptr;

		shader->bind();
		if (!texturePath.empty())
		{
			texture = new Texture(texturePath);
			shader->setUniform1i("u_Texture", 0);
		}
		else
		{
			shader->setUniform4f("u_Color", 
				color.r, color.g, color.b, color.a);
		}

        pVertexArray->bind();
        pVertexArray->bindAttributes(*pVertexBuffer, vbl);

		shader->unBind();
		return new MeshComponent(pVertexArray, pVertexBuffer, pIndexBuffer, shader, texture);
	}

}