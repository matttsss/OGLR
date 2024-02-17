#pragma once

#include "MeshComponent.h"
#include "../utils/Camera.h"
#include "../utils/Terrain.h"

namespace OGLR
{
	class Renderer
	{
	public:
		Renderer();

        void beginFrame(Camera& camera);
        void endFrame();

        void render(Terrain& terrain) const;
		void render(const MeshComponent* mesh, const glm::mat4& modelTransform = glm::mat4(1)) const;

	private:
        struct BGSettings {
            glm::vec3 aim {0, 1, 0};
            int32_t   frameBuffWidth = 0;
            glm::vec3 uBase {-1, 0, 0};
            int32_t   frameBuffHeight = 0;
            glm::vec3 vBase {0, -1, 0};
            int32_t   pad = -1;
        };

		glm::mat4 m_PVMatrix;
        Camera* p_Camera = nullptr;

        Buffer m_BeckGroundUBO;
        BGSettings m_BGSettings;
        MeshComponent m_BackGround;


        static std::vector<uint32_t> s_Indices;
        static std::vector<Vertex<glm::vec3>> s_BackGroundVert;

	};

}



