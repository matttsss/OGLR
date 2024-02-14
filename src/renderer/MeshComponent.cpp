#include "MeshComponent.h"

#include <vector>
#include <unordered_map>
#include <tiny_obj_loader.h>

namespace std
{
    template<>
    struct hash<OGLR::ObjVertex>
    {
        size_t operator()(OGLR::ObjVertex const &vertex) const
        {
            return reinterpret_cast<size_t>(vertex.data);
        }
    };

}


namespace OGLR
{

    static tinyobj::ObjReader reader;
    static tinyobj::ObjReaderConfig reader_config;


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

        std::vector<ObjVertex> vertices;
        std::vector<uint32_t> indices;

        std::unordered_map<ObjVertex, uint32_t> uniqueVertices{};
        for (const auto &shape : shapes) {
            for (const auto &index : shape.mesh.indices) {
                glm::vec3 pos, normal, color;
                glm::vec2 uv;

                if (index.vertex_index >= 0) {
                    pos = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2],
                    };

                    auto colorIndex = 3 * index.vertex_index + 2;
                    if (colorIndex < attrib.colors.size()) {
                        color = {
                                attrib.colors[colorIndex - 2],
                                attrib.colors[colorIndex - 1],
                                attrib.colors[colorIndex - 0],
                        };
                    } else {
                        color = {1.f, 1.f, 1.f};  // set default color
                    }
                }

                if (index.normal_index >= 0) {
                    normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2],
                    };
                }

                if (index.texcoord_index >= 0) {
                    uv = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1],
                    };
                }

                ObjVertex vertex = ObjVertex(pos, normal, color, uv);

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }

        return new MeshComponent(vertices, indices);
    }


    MeshComponent::~MeshComponent()
	{
		delete shader;
	}


    MeshComponent *MeshComponent::addShader(const std::string &vertPath, const std::string &fragPath) {
        shader = Shader::fromGLSLTextFiles(vertPath, fragPath);
        return this;
    }

    void MeshComponent::bind() const {
        shader->bind();
        va.bind();
        ib.bind();

        for (uint32_t i = 0; i < textures.size(); ++i){
            textures[i].bind(i);
            shader->setUniform("u_Texture" + std::to_string(i), i);
        }


    }

    void MeshComponent::unBind() const {
        Texture::unBind();
        Buffer<BufferType::Idx>::unBind();
        VertexArray::unBind();
        Shader::unBind();
    }

    MeshComponent::MeshComponent(Buffer<BufferType::Vtx> &&vb, Buffer<BufferType::Idx> &&ib, VertexArray &&va) noexcept
        : va(std::move(va)), vb(std::move(vb)), ib(std::move(ib)) {}


}