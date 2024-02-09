#pragma once

#include "glm/glm.hpp"
#include <map>

#include "renderer/MeshComponent.h"

typedef OGLR::Vertex<glm::vec2, glm::vec2, glm::vec3> TerrainVertex;

class Terrain {
public:

    /**
     * Initialises builder and compute shader,
     * needs to be called once a valid context is present
     */
    static void initTerrain();

    /***
     * Closes the resources used by the terrain
     */
    static void destroyTerrain();

    /***
     * Builds a terrain with the given resolution and seed
     * @param resolution (int) Number of subdivisions per axis
     * @param seed (int) Seed for random generator
     * @return (Terrain*) Pointer to the newly created terrain
     */
    static OGLR::MeshComponent* buildTile(int32_t resolution, uint32_t seed);


private:

    // Maps resolution to vertex/indices
    static std::map<uint32_t, std::vector<TerrainVertex>> terrainVertices;
    static std::map<uint32_t, std::vector<uint32_t>> terrainIndices;

    static OGLR::Shader* heightComputeShader;
    static OGLR::Shader* normalComputeShader;

};




