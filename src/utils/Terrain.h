#pragma once

#include "glm/glm.hpp"
#include <unordered_map>

#include "buffers/Buffer.h"
#include "renderer/Vertex.h"
#include "renderer/MeshComponent.h"


namespace OGLR {

    typedef Vertex<glm::vec2, glm::vec2, glm::vec3> TerrainVertex;

    struct TerrainSettings {
        uint32_t radius = 4;
        uint32_t resolution = 256;
        uint32_t iter = 1;
        float angle = 0;

        bool operator==(const TerrainSettings &other) const {
            return radius     == other.radius     &&
                   resolution == other.resolution &&
                   iter       == other.iter       &&
                   angle      == other.angle;
        }
    };

    struct TerrainBuffers {
        Buffer<OGLR::BufferType::Vtx> vb;
        Buffer<OGLR::BufferType::Idx> ib;

        VertexArray va;

        TerrainBuffers() = delete;
        TerrainBuffers(const TerrainBuffers&) = delete;
        TerrainBuffers(TerrainBuffers&&) = delete;
        TerrainBuffers(Buffer<OGLR::BufferType::Vtx>&& vb, Buffer<OGLR::BufferType::Idx>&& ib, VertexArray&& va);
    };

    class Terrain {
    public:

        Terrain(const TerrainSettings& settings);

        /***
         * Closes the resources used by the terrain
         */
        static void destroyTerrain();

        void addShader(const std::string& vertexPath, const std::string& fragPath);

        void updateNHMap();
        void updateNHAtPos(const glm::ivec2& tileIdx);
        inline const Texture& getNHTextureAtPos(const glm::ivec2& tileIdx) const {
            return m_NHMaps.at((tileIdx.x + settings.radius) * (2*settings.radius + 1) + (tileIdx.y + settings.radius));
        }

        static TerrainBuffers& getBuffersForRes(uint32_t resolution);

        TerrainSettings settings;
        Shader* renderShader = nullptr;

    private:
        std::vector<Texture> m_NHMaps;

        static std::unordered_map<uint32_t, TerrainBuffers> s_Buffers;

        static OGLR::Shader *s_HeightComputeShader;
        static OGLR::Shader *s_NormalComputeShader;

    };

}
