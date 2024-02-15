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

    struct TerrainSettings {
        GLuint radius = 20;
        GLuint resolution = 64;
        GLuint nbOctaves = 1;
        GLfloat angle = 0;

        TerrainSettings* clamp() {
            radius = glm::clamp(radius, (uint32_t)0, (uint32_t) 64);
            resolution = glm::clamp(resolution, (uint32_t)2, (uint32_t) 512);
            nbOctaves = glm::clamp(nbOctaves, (uint32_t) 1, (uint32_t) 128);
            return this;
        }

        bool operator==(const TerrainSettings &other) const {
            return radius     == other.radius     &&
                   resolution == other.resolution &&
                   nbOctaves  == other.nbOctaves  &&
                   angle      == other.angle;
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
        Terrain(TerrainSettings& settings);
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
        void updateWithSettings(TerrainSettings& settings);


        inline const Buffer& getSettingsUBO() const { return m_Ubo; }

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

        TerrainSettings settings;
        Shader* renderShader = nullptr;

    private:

        void updateNHMap();
        void updateNHAtPos(const glm::ivec2& tileIdx);
        std::unordered_map<glm::ivec2, Texture> m_NHMaps;

        Buffer m_Ubo;

        static void updateBuffersForRes(uint32_t resolution);
        static std::unordered_map<uint32_t, TerrainBuffers> s_Buffers;

        static OGLR::Shader *s_NHComputeShader;

    };

}
