#pragma once

#include <glm/glm.hpp>

#include "MeshComponent.h"

namespace OGLR
{
    class Terrain: public MeshComponent {
    public:

        /***
         * Builds a terrain with the given resolution and seed
         * @param resolution (int) Number of subdivisions per axis
         * @param seed (int) Seed for random generator
         * @return (Terrain*) Pointer to the newly created terrain
         */
        static Terrain* buildTerrain(uint32_t resolution, uint32_t seed);

        static float heightAt(float x, float y, uint32_t seed);

    private:
        Terrain(std::vector<Vertex>* vertices,
                const std::vector<uint32_t>* indices,
                uint32_t resolution, uint32_t seed = 1);

        ~Terrain();

        std::vector<Vertex>* vertices = nullptr;
        const std::vector<uint32_t>* indices = nullptr;

        uint32_t resolution, seed;

    };

}



