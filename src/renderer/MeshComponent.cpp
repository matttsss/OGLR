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


    MeshComponent::MeshComponent(const std::string &objPath, const std::string& shaderPathExtentionless)
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
        for (size_t s = 0; s < shapes.size(); ++s)
        {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f)
            {
                Vertex vert {};
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

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

                shapes[s].mesh.material_ids[f];

                if (uniqueVertices.count(vert) == 0)
                {
                    uniqueVertices[vert] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vert);
                }

                indices.push_back(uniqueVertices[vert]);

            }
        }


        vb = { vertices.data(), static_cast<GLuint>(vertices.size() * sizeof(float)) }; // TODO flatMap vertices to float array
        ib = { indices.data(), static_cast<GLsizei>(indices.size()) };


        shader = Shader::FromGLSLTextFiles(shaderPathExtentionless + ".vert.glsl", shaderPathExtentionless + ".frag.glsl");

        shader->bind();
        // Set uniforms here

        VertexBufferLayout vbl;
        vbl.addFloat(3);
        vbl.addFloat(3);
        vbl.addFloat(3);
        vbl.addFloat(2);


        va.bind();
        va.bindAttributes(vb, vbl);

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
		delete texture;
		delete shader;
	}

}