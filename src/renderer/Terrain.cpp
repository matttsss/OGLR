#include "Terrain.h"

namespace OGLR
{

    Terrain* Terrain::buildTerrain(uint32_t resolution, uint32_t seed) {
        uint32_t pointsNb = resolution * resolution;
        uint32_t indexNb = 80 * 2 * (resolution-1) * (resolution-1);
        std::vector<Vertex>* vertices = new std::vector<Vertex>(pointsNb);
        std::vector<uint32_t>* indices = new std::vector<uint32_t >(indexNb);

        for (uint32_t i = 0; i < resolution; ++i)
        {
            for (uint32_t j = 0; j < resolution; ++j)
            {
                float x = ((float)i)/resolution;
                float y = ((float)j)/resolution;

                uint32_t vertIdx = resolution * j + i;

                vertices->at(vertIdx) = Vertex(
                        {x,0.0f, y}
                );

                //std::cout << "Passed vertices" << std::endl;

                if (i < resolution && j < resolution)
                {
                    const uint32_t indexBfrIdx = 6 * vertIdx;
                    indices->at(indexBfrIdx) = vertIdx;
                    indices->at(indexBfrIdx + 1) = resolution * (j+1) + i;
                    indices->at(indexBfrIdx + 2) = resolution * (j+1) + i + 1;
                    indices->at(indexBfrIdx + 3) = vertIdx;
                    indices->at(indexBfrIdx + 4) = resolution * j + i + 1;
                    indices->at(indexBfrIdx + 5) = resolution * (j+1) + i + 1;
                }
                //std::cout << "Passed indices" << std::endl;


            }
        }


        return new Terrain(vertices, indices, resolution, seed);
    }

    float Terrain::heightAt(float x, float y, uint32_t seed) {
        return 0;
    }

    Terrain::Terrain(std::vector<Vertex> *vertices,
                     const std::vector<uint32_t> *indices,
                     uint32_t resolution, uint32_t seed):
            MeshComponent(*vertices, *indices),
            vertices(vertices), indices(indices),
            resolution(resolution), seed(seed) {}

    Terrain::~Terrain() {
        delete indices;
        delete vertices;
    }


}

