#include "MeshComponent.h"

namespace OGLR
{

	MeshComponent::MeshComponent(const GLfloat* vertices, GLuint sizeOfVertices, const GLuint* indices, GLuint countOfIndices, 
		const Buffers::VertexBufferLayout& vbl, 
		const std::string& vertPath, const std::string& fragPath, const std::string& texPath)
		: vb(vertices, sizeOfVertices), ib(indices, countOfIndices)
	{

		shader = Shader::FromGLSLTextFiles(vertPath, fragPath);
		if (!texPath.empty())
		{
			texture = new Texture(texPath);
			shader->setUniform1i("u_Texture", 0);
		}

		va.bind();
		va.bindAttributes(vb, vbl);

	}

	MeshComponent::~MeshComponent()
	{
		delete texture;
		delete shader;
	}

}