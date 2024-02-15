#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"

#include <unordered_map>

#include "buffers/Buffer.h"
#include "renderer/Vertex.h"
#include "renderer/MeshComponent.h"



namespace OGLR {

    typedef Vertex<glm::vec2, glm::vec3> TerrainVertex;

    struct TerrainSeed {
        GLuint octaves = 1;
        GLfloat angle = 0;
        glm::vec2 pad {0};

        bool operator==(const TerrainSeed &other) const {
            return octaves == other.octaves  &&
                   angle   == other.angle;
        }

        bool operator!=(const TerrainSeed &other) const {
            return octaves != other.octaves  ||
                   angle   != other.angle;
        }
    };

    struct ChunkSettings {
        glm::vec2 centerPos {0};
        glm::vec2 scale {1};
        GLuint resolution = 64;
        glm::vec3 pad {0};

        bool operator==(const ChunkSettings &other) const {
            return resolution == other.resolution &&
                   centerPos  == other.centerPos  &&
                   scale      == other.scale;
        }

        bool operator!=(const ChunkSettings &other) const {
            return resolution != other.resolution ||
                   centerPos  != other.centerPos  ||
                   scale      != other.scale;
        }
    };

    struct TerrainBuffers {
        Buffer vb;
        Buffer ib;

        VertexArray va;

        TerrainBuffers() = delete;
        TerrainBuffers(const TerrainBuffers&) = delete;
        TerrainBuffers(TerrainBuffers&&) = delete;
        TerrainBuffers(Buffer&& vb, Buffer&& ib, VertexArray&& va);
    };

    class Terrain {
    public:

        Terrain() = delete;
        Terrain(Terrain&&) = delete;
        Terrain(const Terrain&) = delete;
        Terrain(const ChunkSettings& cSettings, const TerrainSeed& tSeed);
        ~Terrain();


        /**
         * Closes the static resources used by the terrain
         */
        static void destroyTerrain();

        /**
         * Updates the terrain with the new settings
         * @param settings (Terrain&) New settings for the terrain,
         *  values will be clamped for performance reasons
         */
        void updateSettings(const ChunkSettings& ocSettings, const TerrainSeed &otSeed);


        inline const Buffer& getChunkSettingsUBO() const { return m_ChunkUBO; }
        inline const Buffer& getSeedSettingsUBO()  const { return m_SeedUBO;  }

        /**
         * Provides the normal/height map texture for the given tile coordinates
         * @param tileIdx (const glm::ivec2&) Integer coordinates of the queried tile
         * @return (const Texture&) Reference to the texture
         */
        inline const Texture& getNHTextureAtPos(const glm::ivec2& tileIdx) const {
            return m_NHMaps.at(tileIdx);
        }


        /**
         * Provides the buffers needed to render a flat tile with the given resolution
         * @param resolution (uint32_t) Queried resolution
         * @return (const TerrainBuffers&) Buffers for the resolution
         */
        static const TerrainBuffers& getBuffersForRes(uint32_t resolution);

        TerrainSeed tSeed;
        ChunkSettings cSettings;
        Shader* renderShader = nullptr;

    private:

        void updateNHMap();
        void updateNHAtPos(const glm::ivec2& tileIdx);
        std::unordered_map<glm::ivec2, Texture> m_NHMaps;

        Buffer m_SeedUBO, m_ChunkUBO;

        static void updateBuffersForRes(uint32_t resolution);
        static std::unordered_map<uint32_t, TerrainBuffers> s_Buffers;

        static OGLR::Shader *s_NHComputeShader;

    };

}
