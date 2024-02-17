#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"

#include <unordered_map>

#include "buffers/Buffer.h"
#include "renderer/Vertex.h"
#include "renderer/MeshComponent.h"



namespace OGLR {

    typedef Vertex<glm::vec4, glm::vec4, glm::vec4> ChunkVertex;

    struct TerrainSeed {
        GLuint octaves = 9;
        GLfloat angle = 0.308;
        GLfloat frequ = 15;
        GLfloat zScale = 4;

        bool operator==(const TerrainSeed &other) const {
            return octaves == other.octaves &&
                   angle   == other.angle   &&
                   frequ   == other.frequ   &&
                   zScale  == other.zScale;
        }

        bool operator!=(const TerrainSeed &other) const {
            return octaves != other.octaves ||
                   angle   != other.angle   ||
                   frequ   != other.frequ   ||
                   zScale  != other.zScale;
        }
    };

    struct ChunkSettings {
        glm::vec2 worldOffset {30};
        glm::ivec2 scale {10};
        GLuint resolution = 512;
        glm::vec3 pad {0};

        bool operator==(const ChunkSettings &other) const {
            return resolution  == other.resolution  &&
                   worldOffset == other.worldOffset &&
                   scale       == other.scale;
        }

        bool operator!=(const ChunkSettings &other) const {
            return resolution  != other.resolution  ||
                   worldOffset != other.worldOffset ||
                   scale       != other.scale;
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

    struct TerrainQTree {

        TerrainQTree(const glm::vec2& min, const glm::vec2& max);



    private:
        glm::vec2 min {0}, max {0};

        TerrainBuffers* buffers = nullptr;
        TerrainQTree* children[4] {nullptr, nullptr, nullptr, nullptr};
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
         * @param ocSettings (const ChunkSettings&) New settings for the chunks
         * @param otSeed     (const TerrainSeed&)   New settings for terrain seed
         */
        void updateSettings(const ChunkSettings& ocSettings, const TerrainSeed &otSeed);

        void onUpdate(const glm::vec3& posInWorld);

        inline const std::unordered_map<glm::ivec2, TerrainBuffers>& getChunks() const { return m_Chunks; }
        inline const Buffer& getChunkSettingsUBO() const { return m_ChunkUBO; }
        inline const Buffer& getSeedSettingsUBO()  const { return m_SeedUBO;  }


        TerrainSeed tSeed;
        ChunkSettings cSettings;

        Shader* chunkRenderer = nullptr;

        void updateChunkAt(const glm::ivec2& chunkIdx);
        TerrainBuffers& getChunk(const glm::ivec2& chunkIdx);

    private:
        Buffer m_SeedUBO, m_ChunkUBO;
        std::unordered_map<glm::ivec2, TerrainBuffers> m_Chunks;


        static Shader* s_MeshMaker;
    };

}
