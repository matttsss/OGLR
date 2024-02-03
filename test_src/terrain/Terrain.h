#pragma once

#include "glm/glm.hpp"
#include <map>

#include "renderer/MeshComponent.h"


class Terrain {
public:

    /***
     * Builds a terrain with the given resolution and seed
     * @param resolution (int) Number of subdivisions per axis
     * @param seed (int) Seed for random generator
     * @return (Terrain*) Pointer to the newly created terrain
     */
    static OGLR::MeshComponent* buildTile(uint32_t resolution, uint32_t seed);

    static float heightAt(float x, float y, uint32_t seed);

private:

    // Maps resolution to vertex/indices
    static std::map<uint32_t, std::vector<OGLR::Vertex>> terrainVertices;
    static std::map<uint32_t, std::vector<uint32_t>> terrainIndices;

};




