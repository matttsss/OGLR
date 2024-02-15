#include "Terrain.h"

namespace OGLR {

    std::unordered_map<uint32_t, TerrainBuffers> Terrain::s_Buffers;

    OGLR::Shader *Terrain::s_NHComputeShader;

    Terrain::Terrain(const ChunkSettings& cSettings, const TerrainSeed &tSeed)
        : tSeed(tSeed), cSettings(cSettings),
          m_SeedUBO(BufType::UBO, &tSeed, sizeof(TerrainSeed), UsageType::Dynamic),
          m_ChunkUBO(BufType::UBO, &cSettings, sizeof(ChunkSettings), UsageType::Dynamic) {

        if (!s_NHComputeShader)
            s_NHComputeShader = Shader::fromGLSLTextFiles("test_res/shaders/compute_NHMap.glsl");

        updateNHMap();
        updateBuffersForRes(cSettings.resolution);
    }

    Terrain::~Terrain() {
        delete renderShader;
    }

    void Terrain::destroyTerrain() {
        delete s_NHComputeShader;
    }


    void Terrain::updateSettings(const ChunkSettings& ocSettings, const TerrainSeed &otSeed) {
        if (cSettings == ocSettings && tSeed == otSeed) return;
        if (cSettings != ocSettings) {
            cSettings = ocSettings;
            m_ChunkUBO.bind();
            m_ChunkUBO.setData(&cSettings, sizeof(ChunkSettings));
            m_ChunkUBO.unBind();
        }
        if (tSeed != otSeed) {
            tSeed = otSeed;
            m_SeedUBO.bind();
            m_SeedUBO.setData(&tSeed, sizeof(TerrainSeed));
            m_SeedUBO.unBind();
        }

        updateNHMap();

        updateBuffersForRes(cSettings.resolution);
    }

    const TerrainBuffers& Terrain::getBuffersForRes(uint32_t resolution) {
        updateBuffersForRes(resolution);
        return s_Buffers.at(resolution);
    }


    void Terrain::updateNHMap() {
        int32_t radius = 5;
        m_NHMaps.clear();
        m_NHMaps.reserve((2*radius + 1) * (2*radius + 1));
        for (int32_t i = -radius; i <= radius; ++i)
            for (int32_t j = -radius; j <= radius; ++j)
                updateNHAtPos({i, j});
    }

    void Terrain::updateNHAtPos(const glm::ivec2 &tileIdx) {
        uint32_t resolution = cSettings.resolution;
        Texture NHMap(nullptr, OGLR::Texture::Type::X4f, resolution, resolution);

        // Launch computation
        m_SeedUBO.bind();
        s_NHComputeShader->bind();

        s_NHComputeShader->setUniformBlock("u_TerrainSettings", m_SeedUBO);
        s_NHComputeShader->setUniformBlock("u_ChunkSettings", m_ChunkUBO);

        NHMap.bind();
        NHMap.bindAsImage(0, GL_WRITE_ONLY);

        s_NHComputeShader->setUniform("u_NHMap", 0);
        s_NHComputeShader->setUniform("u_Offset", tileIdx);

        glDispatchCompute((resolution + 7) / 8, (resolution + 7) / 8, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        Shader::unBind();
        m_SeedUBO.unBind();
        Texture::unBind();
        m_NHMaps.emplace(std::piecewise_construct,
                         std::forward_as_tuple(tileIdx),
                         std::forward_as_tuple(std::move(NHMap)));
    }

    void Terrain::updateBuffersForRes(uint32_t resolution) {
        if (s_Buffers.count(resolution)) return;

        uint32_t pointsNb = resolution * resolution;
        uint32_t indexNb = 3 * 2 * pointsNb;
        std::vector<TerrainVertex> vertices = std::vector<TerrainVertex>(pointsNb);
        std::vector<uint32_t> indices = std::vector<uint32_t>(indexNb);

        for (uint32_t i = 0; i < resolution; ++i) {
            for (uint32_t j = 0; j < resolution; ++j) {
                float x = ((float) i) / resolution;
                float z = ((float) j) / resolution;

                uint32_t vertIdx = resolution * j + i;

                vertices.at(vertIdx) = TerrainVertex(
                        {x, z},
                        {.7f, .7f, .7f}
                );

                if (i < resolution - 1 && j < resolution - 1) {
                    indices.push_back(vertIdx);
                    indices.push_back(resolution * (j + 1) + i);
                    indices.push_back(resolution * (j + 1) + i + 1);
                    indices.push_back(vertIdx);
                    indices.push_back(resolution * (j + 1) + i + 1);
                    indices.push_back(resolution * j + i + 1);
                }

            }
        }

        s_Buffers.emplace(std::piecewise_construct,
                          std::forward_as_tuple(resolution),
                          std::forward_as_tuple(
                                  Buffer(BufType::VBO, vertices.data(), vertices.size() * sizeof(TerrainVertex)),
                                  Buffer(BufType::IBO, indices.data(), indices.size() * sizeof(uint32_t)),
                                  VertexArray()));

        TerrainBuffers& tb = s_Buffers.at(resolution);

        tb.vb.bind();
        tb.va.bind();

        tb.va.bindAttributes<TerrainVertex>();

        tb.vb.unBind();
        VertexArray::unBind();
    }


    TerrainBuffers::TerrainBuffers(Buffer &&vb, Buffer &&ib, VertexArray &&va)
        : vb(std::move(vb)), ib(std::move(ib)), va(std::move(va)){}

}



