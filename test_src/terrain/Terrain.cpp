#include "Terrain.h"

std::map<uint32_t, std::vector<OGLR::Vertex>> Terrain::terrainVertices;
std::map<uint32_t, std::vector<uint32_t>> Terrain::terrainIndices;
OGLR::Shader* Terrain::computeShader;

void Terrain::initTerrain() {
    computeShader = OGLR::Shader::FromGLSLTextFiles("test_res/shaders/compute_grad_height.glsl");
}

void Terrain::destroyTerrain() {
    delete computeShader;
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

    OGLR::Texture grad_height = OGLR::Texture(nullptr, OGLR::Texture::Type::X4f, resolution, resolution);

    // Launch computations
    computeShader->bind();
    computeShader->setUniformMat4f("u_Transform", glm::mat4(1.0f));
    grad_height.bindAsImage(GL_WRITE_ONLY);
    glDispatchCompute(resolution, resolution, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    computeShader->unBind();

    // Will hold gradient values in first three components and height in the last one
    mesh->addTexture(std::move(grad_height));


    return mesh;
}



