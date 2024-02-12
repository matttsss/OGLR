#include "Terrain.h"

std::unordered_map<uint32_t, std::vector<TerrainVertex>> Terrain::terrainVertices;
std::unordered_map<uint32_t, std::vector<uint32_t>> Terrain::terrainIndices;
OGLR::Shader* Terrain::heightComputeShader;
OGLR::Shader* Terrain::normalComputeShader;

void Terrain::initTerrain() {
    heightComputeShader = OGLR::Shader::FromGLSLTextFiles("test_res/shaders/compute_height.glsl");
    normalComputeShader = OGLR::Shader::FromGLSLTextFiles("test_res/shaders/compute_normal.glsl");
}

void Terrain::destroyTerrain() {
    delete heightComputeShader;
    delete normalComputeShader;
}

OGLR::MeshComponent* Terrain::buildTile(const TerrainSettings& settings) {
    OGLR::MeshComponent* mesh = nullptr;

    uint32_t resolution = settings.resolution;

    // If chosen resolution is in cache, fetch it:
    if (terrainVertices.count(resolution) && terrainIndices.count(resolution))
        mesh = new OGLR::MeshComponent(terrainVertices[resolution], terrainIndices[resolution]);

    // Else we create it:
    else {
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
                    const uint32_t indexBfrIdx = 6 * vertIdx;
                    indices.at(indexBfrIdx) = vertIdx;
                    indices.at(indexBfrIdx + 1) = resolution * (j + 1) + i;
                    indices.at(indexBfrIdx + 2) = resolution * (j + 1) + i + 1;
                    indices.at(indexBfrIdx + 3) = vertIdx;
                    indices.at(indexBfrIdx + 4) = resolution * (j + 1) + i + 1;
                    indices.at(indexBfrIdx + 5) = resolution * j + i + 1;
                }

            }
        }

        mesh = new OGLR::MeshComponent(vertices, indices);

        terrainVertices[resolution] = std::move(vertices);
        terrainIndices[resolution]  = std::move(indices);

    }

    // Bind shader
    mesh->addShader("test_res/shaders/terrain_shader.vert.glsl", "test_res/shaders/terrain_shader.frag.glsl");


    OGLR::Texture height (nullptr, OGLR::Texture::Type::X1f, resolution, resolution);
    OGLR::Texture normal (nullptr, OGLR::Texture::Type::X4f, resolution, resolution);

    // Launch computations
    heightComputeShader->bind();

    height.bind();
    height.bindAsImage(0, GL_WRITE_ONLY);

    heightComputeShader->setUniform1i("u_Texture0", 0);
    heightComputeShader->setUniformVec2i("u_Offset", 0, 0);

    glDispatchCompute((resolution + 7) / 8, (resolution + 7) / 8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    heightComputeShader->unBind();


    normalComputeShader->bind();

    height.bind(0);
    height.bindAsImage(0, GL_READ_ONLY);

    normal.bind(1);
    normal.bindAsImage(1, GL_WRITE_ONLY);

    normalComputeShader->setUniform1i("u_Texture0", 0);
    normalComputeShader->setUniform1i("u_Texture1", 1);

    glDispatchCompute((resolution + 7) / 8, (resolution + 7) / 8, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    normalComputeShader->unBind();


    // Will hold gradient values in first three components and height in the last one
    mesh->addTexture(std::move(height));
    mesh->addTexture(std::move(normal));

    return mesh;
}



