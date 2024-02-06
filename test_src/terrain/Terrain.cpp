#include "Terrain.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

std::map<uint32_t, std::vector<OGLR::Vertex>> Terrain::terrainVertices;
std::map<uint32_t, std::vector<uint32_t>> Terrain::terrainIndices;


OGLR::MeshComponent* Terrain::buildTile(int32_t resolution, uint32_t seed) {
    OGLR::MeshComponent* mesh = nullptr;

    // If chosen resolution is in cache, fetch it:
    if (terrainVertices.count(resolution) && terrainIndices.count(resolution))
        mesh = new OGLR::MeshComponent(terrainVertices[resolution], terrainIndices[resolution]);

    // Else we create it:
    else {
        uint32_t pointsNb = resolution * resolution;
        uint32_t indexNb = 3 * 2 * pointsNb;
        std::vector<OGLR::Vertex> vertices = std::vector<OGLR::Vertex>(pointsNb);
        std::vector<uint32_t> indices = std::vector<uint32_t>(indexNb);

        for (uint32_t i = 0; i < resolution; ++i) {
            for (uint32_t j = 0; j < resolution; ++j) {
                float x = ((float) i) / resolution;
                float z = ((float) j) / resolution;

                uint32_t vertIdx = resolution * j + i;

                vertices.at(vertIdx) = OGLR::Vertex(
                        {x, 0.0f, z},
                        {0.0f, 1.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        {x, z}
                );

                if (i < resolution - 1 && j < resolution - 1) {
                    const uint32_t indexBfrIdx = 6 * vertIdx;
                    indices.at(indexBfrIdx) = vertIdx;
                    indices.at(indexBfrIdx + 1) = resolution * (j + 1) + i;
                    indices.at(indexBfrIdx + 2) = resolution * (j + 1) + i + 1;
                    indices.at(indexBfrIdx + 3) = vertIdx;
                    indices.at(indexBfrIdx + 4) = resolution * (j + 1) + i + 1;
                    indices.at(indexBfrIdx + 5) = resolution * j + i + 1;
                }

            }
        }

        mesh = new OGLR::MeshComponent(vertices, indices);

        terrainVertices[resolution] = std::move(vertices);
        terrainIndices[resolution]  = std::move(indices);

    }

    // Bind shader
    mesh->addShader("test_res/shaders/terrain_shader.vert.glsl", "test_res/shaders/terrain_shader.frag.glsl");

    // Create and load height map / gradient map

    float* heightMap = new float[resolution * resolution];

    static_assert(sizeof(glm::vec3) == sizeof(GLfloat) * 3, "Platform doesn't support this directly.");
    glm::vec3* gradientMap = new glm::vec3[resolution * resolution];

    for (int i = 0; i < resolution; ++i)
    {
        for (int j = 0; j < resolution; ++j)
        {
            float x = (float)i/resolution;
            float z = (float)j/resolution;

            heightMap[j*resolution + i] = heightAt(x, z, seed);
            gradientMap[j*resolution + i] = glm::normalize(glm::vec3{z - 0.5f, x - 0.5f, 1.0f});
        }
    }

    mesh->addTexture(OGLR::Texture(heightMap, OGLR::Texture::Type::X1f, resolution, resolution));
    mesh->addTexture(OGLR::Texture(gradientMap, OGLR::Texture::Type::X3f, resolution, resolution));

    return mesh;
}

float Terrain::heightAt(float x, float y, uint32_t seed) {
    //return 0.5f;
    return (x-0.5f)*(y-0.5f) + 0.5f;
}


