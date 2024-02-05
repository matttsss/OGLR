#include "Terrain.h"


std::map<uint32_t, std::vector<OGLR::Vertex>> Terrain::terrainVertices;
std::map<uint32_t, std::vector<uint32_t>> Terrain::terrainIndices;


OGLR::MeshComponent* Terrain::buildTile(uint32_t resolution, uint32_t seed) {
    OGLR::MeshComponent* mesh = nullptr;

    if (terrainVertices.count(resolution) && terrainIndices.count(resolution))
        mesh = new OGLR::MeshComponent(terrainVertices[resolution], terrainIndices[resolution]);
    else {
        uint32_t pointsNb = resolution * resolution;
        uint32_t indexNb = 3 * 2 * pointsNb;
        std::vector<OGLR::Vertex> vertices = std::vector<OGLR::Vertex>(pointsNb);
        std::vector<uint32_t> indices = std::vector<uint32_t>(indexNb);

        for (uint32_t i = 0; i < resolution; ++i) {
            for (uint32_t j = 0; j < resolution; ++j) {
                float x = ((float) i) / resolution - 0.5f;
                float z = ((float) j) / resolution - 0.5f;

                uint32_t vertIdx = resolution * j + i;

                vertices.at(vertIdx) = OGLR::Vertex(
                        {x, heightAt(x, z, seed), z},
                        {0.0f, 1.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        {x + .5f, z + .5f}
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


    mesh->addShader("test_res/shaders/terrain_shader.vert.glsl", "test_res/shaders/terrain_shader.frag.glsl");

    // Create and load height map

    float heightMap[resolution * resolution];

    for (int i = 0; i < resolution * resolution; ++i)
            heightMap[i] = heightAt((float)(i % resolution)/resolution - 0.5f, (float)(i - i/resolution * resolution)/resolution - 0.5f, seed);

    mesh->addTexture(OGLR::Texture(heightMap, OGLR::Texture::Type::X1f, resolution, resolution));

    return mesh;
}

float Terrain::heightAt(float x, float y, uint32_t seed) {
    return sinf(x*y) + 1;
}


