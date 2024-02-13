#include "Terrain.h"

namespace OGLR {

    std::unordered_map<uint32_t, TerrainBuffers> Terrain::s_Buffers;

    std::unordered_map<uint32_t, std::vector<TerrainVertex>> Terrain::terrainVertices;
    std::unordered_map<uint32_t, std::vector<uint32_t>> Terrain::terrainIndices;

    OGLR::Shader *Terrain::s_HeightComputeShader;
    OGLR::Shader *Terrain::s_NormalComputeShader;

    void Terrain::initTerrain() {
        s_HeightComputeShader = OGLR::Shader::FromGLSLTextFiles("test_res/shaders/compute_height.glsl");
        s_NormalComputeShader = OGLR::Shader::FromGLSLTextFiles("test_res/shaders/compute_normal.glsl");
    }

    void Terrain::destroyTerrain() {
        delete s_HeightComputeShader;
        delete s_NormalComputeShader;
    }

    MeshComponent *Terrain::buildTile(const TerrainSettings &settings) {

        uint32_t resolution = settings.resolution;

        Texture height(nullptr, OGLR::Texture::Type::X1f, resolution, resolution);
        Texture NHMap(nullptr, OGLR::Texture::Type::X4f, resolution, resolution);

        // Launch computations
        s_HeightComputeShader->bind();

        height.bind();
        height.bindAsImage(0, GL_WRITE_ONLY);

        s_HeightComputeShader->setUniform1i("u_HeightMap", 0);
        s_HeightComputeShader->setUniformVec2i("u_Offset", 0, 0);

        glDispatchCompute((resolution + 7) / 8, (resolution + 7) / 8, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        s_HeightComputeShader->unBind();


        s_NormalComputeShader->bind();

        height.bind(0);
        height.bindAsImage(0, GL_READ_ONLY);

        NHMap.bind(1);
        NHMap.bindAsImage(1, GL_WRITE_ONLY);

        s_NormalComputeShader->setUniform1i("u_HeightMap", 0);
        s_NormalComputeShader->setUniform1i("u_NHMap", 1);

        glDispatchCompute((resolution + 7) / 8, (resolution + 7) / 8, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        s_NormalComputeShader->unBind();

        TerrainBuffers* ptb = extractBuffersForRes(resolution);
        MeshComponent* mesh = new MeshComponent(std::move(ptb->vb), std::move(ptb->ib), std::move(ptb->va));
        delete ptb;

        // Will hold gradient values in first three components and height in the last one
        mesh->addTexture(std::move(NHMap));

        mesh->addShader("test_res/shaders/terrain_shader.vert.glsl", "test_res/shaders/terrain_shader.frag.glsl");

        return mesh;
    }


    TerrainBuffers& Terrain::getBuffersForRes(uint32_t resolution) {
        if (s_Buffers.count(resolution)) return s_Buffers.at(resolution);

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
                                  Buffer<BufferType::Vtx>(vertices.data(), vertices.size() * sizeof(TerrainVertex)),
                                  Buffer<BufferType::Idx>(indices.data(), indices.size() * sizeof(uint32_t)),
                                  VertexArray()));

        TerrainBuffers& tb = s_Buffers.at(resolution);

        VertexBufferLayout vbl;
        vbl.addAttr<glm::vec2>();
        vbl.addAttr<glm::vec2>();
        vbl.addAttr<glm::vec3>();

        tb.vb.bind();
        tb.va.bind();

        tb.va.bindAttributes(vbl);

        Buffer<BufferType::Vtx>::unBind();
        VertexArray::unBind();

        return tb;
    }

    TerrainBuffers* Terrain::extractBuffersForRes(uint32_t resolution) {
        TerrainBuffers* tb = new TerrainBuffers(std::move(getBuffersForRes(resolution)));
        s_Buffers.erase(resolution);
        return tb;
    }

    TerrainBuffers::TerrainBuffers(TerrainBuffers &&other) noexcept
        : TerrainBuffers(std::move(other.vb), std::move(other.ib), std::move(other.va)) {}

    TerrainBuffers::TerrainBuffers(Buffer<OGLR::BufferType::Vtx> &&vb, Buffer<OGLR::BufferType::Idx> &&ib, VertexArray &&va)
        : vb(std::move(vb)), ib(std::move(ib)), va(std::move(va)){}

}



