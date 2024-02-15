#include "Terrain.h"

namespace OGLR {

    Shader *Terrain::s_MeshMaker;

    Terrain::Terrain(const ChunkSettings& cSettings, const TerrainSeed &tSeed)
        : tSeed(tSeed), cSettings(cSettings),
          m_SeedUBO(BufType::UBO, &tSeed, sizeof(TerrainSeed), UsageType::Dynamic),
          m_ChunkUBO(BufType::UBO, &cSettings, sizeof(ChunkSettings), UsageType::Dynamic) {

        if (!s_MeshMaker)
            s_MeshMaker = Shader::fromGLSLTextFiles("test_res/shaders/compute_vertices.glsl");

    }

    Terrain::~Terrain() {
        delete chunkRenderer;
    }

    void Terrain::destroyTerrain() {
        delete s_MeshMaker;
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

        m_Chunks.clear();
    }

    MeshComponent &Terrain::getChunk(const glm::ivec2 &chunkIdx) {
        if (m_Chunks.count(chunkIdx) != 0)
            return m_Chunks.at(chunkIdx);

        uint32_t resolution = cSettings.resolution;

        Buffer idxSSBO {BufType::SSBO, nullptr,
                        static_cast<GLuint>(resolution * resolution * 6 * sizeof(uint32_t)),
                        UsageType::Dynamic};
        Buffer vertSSBO {BufType::SSBO, nullptr,
                         static_cast<GLuint>((resolution + 1) * (resolution + 1) * sizeof(ChunkVertex)),
                         UsageType::Dynamic};

        // Launch computation
        s_MeshMaker->bind();
        m_SeedUBO.bind();
        s_MeshMaker->setUniformBlock("u_TerrainSettings", m_SeedUBO);

        m_ChunkUBO.bind();
        s_MeshMaker->setUniformBlock("u_ChunkSettings", m_ChunkUBO);

        vertSSBO.bindAsBufferBase(0);
        idxSSBO.bindAsBufferBase(1);

        glDispatchCompute(ceilf32((float)(resolution + 1) / 8), ceilf32((float)(resolution + 1) / 8), 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        Shader::unBind();
        m_ChunkUBO.unBind();

        VertexArray va;
        idxSSBO.castTo(BufType::IBO, UsageType::Static);
        vertSSBO.castTo(BufType::VBO, UsageType::Static);

        va.bind();
        vertSSBO.bind();

        va.bindAttributes<ChunkVertex>();

        vertSSBO.unBind();
        VertexArray::unBind();

        m_Chunks.emplace(std::piecewise_construct,
                         std::forward_as_tuple(chunkIdx),
                         std::forward_as_tuple(std::move(vertSSBO), std::move(idxSSBO), std::move(va)));

        MeshComponent& chunk = m_Chunks.at(chunkIdx);

        return chunk;
    }


    TerrainBuffers::TerrainBuffers(Buffer &&vb, Buffer &&ib, VertexArray &&va)
        : vb(std::move(vb)), ib(std::move(ib)), va(std::move(va)){}

}



