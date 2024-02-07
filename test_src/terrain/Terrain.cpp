#include "Terrain.h"

std::map<uint32_t, std::vector<OGLR::Vertex>> Terrain::terrainVertices;
std::map<uint32_t, std::vector<uint32_t>> Terrain::terrainIndices;
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

OGLR::MeshComponent* Terrain::buildTile(int32_t resolution, uint32_t seed) {
    OGLR::MeshComponent* mesh = nullptr;

    // If chosen resolution is in cache, fetch it:
    if (terrainVertices.count(resolution) && terrainIndices.count(resolution))
        mesh = new OGLR::MeshComponent(terrainVertices[resolution], terrainIndices[resolution]);

    // Else we create it:
    else {
        uint32_t pointsNb = resolution * resolution;
        uint32_t indexNb = 3 * 2 * pointsNb;
        std::vector<OGLR::Vertex> vertices = std::vector<OGLR::Vertex>(pointsNb);
        std::vector<uint32_t> indices = std::vector<uint32_t>(indexNb);

        for (uint32_t i = 0; i < resolution; ++i) {
            for (uint32_t j = 0; j < resolution; ++j) {
                float x = ((float) i) / resolution;
                float z = ((float) j) / resolution;

                uint32_t vertIdx = resolution * j + i;

                vertices.at(vertIdx) = OGLR::Vertex(
                        {x, 0.0f, z},
                        {0.0f, 1.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        {x, z}
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

    // Create and load height map / gradient map

    //float* heightMap = new float[resolution * resolution];
//
    //static_assert(sizeof(glm::vec3) == sizeof(GLfloat) * 3, "Platform doesn't support this directly.");
    //glm::vec3* gradientMap = new glm::vec3[resolution * resolution];
//
    //for (int i = 0; i < resolution; ++i)
    //{
    //    for (int j = 0; j < resolution; ++j)
    //    {
    //        float x = (float)i/resolution;
    //        float z = (float)j/resolution;
//
    //        heightMap[j*resolution + i] = heightAt(x, z, seed);
    //        gradientMap[j*resolution + i] = glm::normalize(glm::vec3{z - 0.5f, x - 0.5f, 1.0f});
    //    }
    //}

    OGLR::Texture height = OGLR::Texture(nullptr, OGLR::Texture::Type::X1f, resolution, resolution);
    OGLR::Texture normal = OGLR::Texture(nullptr, OGLR::Texture::Type::X4f, resolution, resolution);

    // Launch computations
    heightComputeShader->bind();

    height.bind();
    height.bindAsImage(GL_WRITE_ONLY);

    heightComputeShader->setUniform1i("u_Texture0", 0);
    heightComputeShader->setUniformMat4f("u_Transform", glm::mat4(1.0f));

    glDispatchCompute(resolution, resolution, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    heightComputeShader->unBind();



    normalComputeShader->bind();

    height.bind(0);
    height.bindAsImage(GL_READ_ONLY);

    normal.bind(1);
    normal.bindAsImage(GL_WRITE_ONLY);

    normalComputeShader->setUniform1i("u_Texture0", 0);
    normalComputeShader->setUniform1i("u_Texture1", 1);

    glDispatchCompute(resolution, resolution, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    normalComputeShader->unBind();

    // Will hold gradient values in first three components and height in the last one
    mesh->addTexture(std::move(height));
    mesh->addTexture(std::move(normal));

    return mesh;
}



