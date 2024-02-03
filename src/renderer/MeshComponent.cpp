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


    MeshComponent::MeshComponent(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
        : va(), vb(vertices.data(), vertices.size() * Vertex::ATTRIBUTES_SIZE * sizeof(float)),
          ib(indices.data(), indices.size())
    {

        VertexBufferLayout vbl;
        vbl.addFloat(3);
        vbl.addFloat(3);
        vbl.addFloat(3);
        vbl.addFloat(2);

        vb.bind();
        ib.bind();

        va.bind();
        va.bindAttributes(vb, vbl);

    }

    MeshComponent* MeshComponent::loadFromObjFile(const std::string &objPath)
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
        //auto& materials = reader.GetMaterials();

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};
        for (const auto &shape : shapes) {
            for (const auto &index : shape.mesh.indices) {
                Vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2],
                    };

                    auto colorIndex = 3 * index.vertex_index + 2;
                    if (colorIndex < attrib.colors.size()) {
                        vertex.color = {
                                attrib.colors[colorIndex - 2],
                                attrib.colors[colorIndex - 1],
                                attrib.colors[colorIndex - 0],
                        };
                    } else {
                        vertex.color = {1.f, 1.f, 1.f};  // set default color
                    }
                }

                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2],
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1],
                    };
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }

        return new MeshComponent(vertices, indices);

        /*
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
        */

    }


    MeshComponent::~MeshComponent()
	{
		delete shader;
	}


    MeshComponent *MeshComponent::addTexture(const std::string &texPath) {
        textures.emplace_back(texPath);
        return this;
    }

    MeshComponent *MeshComponent::addShader(const std::string &vertPath, const std::string &fragPath) {
        shader = OGLR::Shader::FromGLSLTextFiles(vertPath, fragPath);
        return this;
    }

    void MeshComponent::bind() const {
        shader->bind();
        va.bind();
        ib.bind();

        for (uint32_t i = 0; i < textures.size(); ++i)
            textures[i].bind(i);

    }

    void MeshComponent::unBind() const {
        for (const Texture& tex : textures)
            tex.unBind();

        ib.unBind();
        va.unBind();
        shader->unBind();
    }

}