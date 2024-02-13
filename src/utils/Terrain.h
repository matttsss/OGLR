#pragma once

#include "glm/glm.hpp"
#include <unordered_map>

#include "buffers/Buffer.h"
#include "renderer/MeshComponent.h"


namespace OGLR {

    typedef Vertex<glm::vec2, glm::vec2, glm::vec3> TerrainVertex;

    struct TerrainSettings {
        uint32_t resolution = 256;
        uint32_t iter = 1;
        float angle = 0;

        bool operator==(const TerrainSettings &other) const {
            return resolution == other.resolution &&
                   iter == other.iter &&
                   angle == other.angle;
        }
    };

    struct TerrainBuffers {
        Buffer<OGLR::BufferType::Vtx> vb;
        Buffer<OGLR::BufferType::Idx> ib;

        VertexArray va;

        TerrainBuffers() = delete;
        TerrainBuffers(const TerrainBuffers&) = delete;
        TerrainBuffers(TerrainBuffers&& other) noexcept;
        TerrainBuffers(Buffer<OGLR::BufferType::Vtx>&& vb, Buffer<OGLR::BufferType::Idx>&& ib, VertexArray&& va);
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
        static OGLR::MeshComponent *buildTile(const TerrainSettings &settings);

        static TerrainBuffers& getBuffersForRes(uint32_t resolution);
        static TerrainBuffers* extractBuffersForRes(uint32_t resolution);

    private:

        static std::unordered_map<uint32_t, TerrainBuffers> s_Buffers;

        // Maps resolution to vertex/indices
        static std::unordered_map<uint32_t, std::vector<TerrainVertex>> terrainVertices;
        static std::unordered_map<uint32_t, std::vector<uint32_t>> terrainIndices;

        static OGLR::Shader *s_HeightComputeShader;
        static OGLR::Shader *s_NormalComputeShader;

    };

}
