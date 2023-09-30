#include "MeshComponent.h"

#include <vector>
#include <tiny_obj_loader.h>

using namespace OGLR::Buffers;

namespace OGLR
{


	MeshComponent::MeshComponent(VertexArray* va, VertexBuffer* vb, IndexBuffer* ib,
		Shader* shader, Texture* texture)
		: va(va), vb(vb), ib(ib), shader(shader), texture(texture)
	{
	}

    MeshComponent::MeshComponent(const std::string &objPath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str()))
            throw std::runtime_error(warn + err);

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {

            }
        }


    }



    MeshComponent::~MeshComponent()
	{
		delete va;
		delete vb;
		delete ib;
		delete texture;
		delete shader;
	}

    void MeshComponent::MeshComponentBuilder::setVertices(GLfloat* vertices, GLuint verticesSize)
	{
		this->vertices = vertices;
		this->verticesSize = verticesSize;
	}

	void MeshComponent::MeshComponentBuilder::setVerticesLayout(VertexBufferLayout& vbl)
	{
		this->vbl = vbl;
	}

	void MeshComponent::MeshComponentBuilder::setIndices(GLuint* indices, GLsizei indicesCount)
	{
		this->indices = indices;
		this->indicesCount = indicesCount;
	}

	void MeshComponent::MeshComponentBuilder::setShaderPath(const std::string& vertPath, const std::string& fragPath)
	{
		this->vertPath = vertPath;
		this->fragPath = fragPath;
	}

	void MeshComponent::MeshComponentBuilder::setTexturePath(const std::string& texPath)
	{
		this->texturePath = texPath;
	}

	void MeshComponent::MeshComponentBuilder::setColor(const glm::vec4& color)
	{
		this->color = color;
	}

	MeshComponent* MeshComponent::MeshComponentBuilder::build()
	{

        VertexArray* pVertexArray = new VertexArray();
        VertexBuffer* pVertexBuffer = new VertexBuffer(vertices, verticesSize);
        IndexBuffer* pIndexBuffer = new IndexBuffer(indices, indicesCount);


		auto* shader = Shader::FromGLSLTextFiles(vertPath, fragPath);
		Texture* texture = nullptr;

		shader->bind();
		if (!texturePath.empty())
		{
			texture = new Texture(texturePath);
			shader->setUniform1i("u_Texture", 0);
		}
		else
		{
			shader->setUniform4f("u_Color", 
				color.r, color.g, color.b, color.a);
		}

        pVertexArray->bind();
        pVertexArray->bindAttributes(*pVertexBuffer, vbl);

		shader->unBind();
		return new MeshComponent(pVertexArray, pVertexBuffer, pIndexBuffer, shader, texture);
	}

}