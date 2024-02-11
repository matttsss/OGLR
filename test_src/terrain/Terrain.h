#pragma once

#include "glm/glm.hpp"
#include <map>

#include "renderer/MeshComponent.h"

typedef OGLR::Vertex<glm::vec2, glm::vec2, glm::vec3> TerrainVertex;

struct TerrainSettings {
    uint32_t resolution = 256;
    uint32_t iter = 1;
    float angle = 0;

    bool operator==(const TerrainSettings& other) const {
        return resolution == other.resolution &&
               iter       == other.iter       &&
               angle      == other.angle;
    }
};

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
     * @return (OGLR::MeshComponent*) Pointer to the newly created terrain
     */
    static OGLR::MeshComponent* buildTile(const TerrainSettings& settings);


private:

    // Maps resolution to vertex/indices
    static std::map<uint32_t, std::vector<TerrainVertex>> terrainVertices;
    static std::map<uint32_t, std::vector<uint32_t>> terrainIndices;

    static OGLR::Shader* heightComputeShader;
    static OGLR::Shader* normalComputeShader;

};




