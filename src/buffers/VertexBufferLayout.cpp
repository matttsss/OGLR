#include "VertexBufferLayout.h"

#include <glm/glm.hpp>

namespace OGLR::Buffers
{
    VertexBufferLayout::VertexAttribute::VertexAttribute(GLint count, GLenum type, GLboolean normalised)
            : count(count), type(type), normalised(normalised) {}

    template<>
    void VertexBufferLayout::addAttr<glm::vec3>() {
        m_Attributes.emplace_back(3, GL_FLOAT, GL_FALSE);
        m_Stride += sizeof(glm::vec3);
    }

    template<>
    void VertexBufferLayout::addAttr<glm::vec2>() {
        m_Attributes.emplace_back(2, GL_FLOAT, GL_FALSE);
        m_Stride += sizeof(glm::vec2);
    }

    template<>
    void VertexBufferLayout::addAttr<glm::ivec3>() {
        m_Attributes.emplace_back(3, GL_INT, GL_FALSE);
        m_Stride += sizeof(glm::ivec3);
    }

    template<>
    void VertexBufferLayout::addAttr<glm::ivec2>() {
        m_Attributes.emplace_back(2, GL_INT, GL_FALSE);
        m_Stride += sizeof(glm::ivec2);
    }

}