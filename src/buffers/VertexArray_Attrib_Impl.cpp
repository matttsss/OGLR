#include "VertexArray.h"

#include <glm/glm.hpp>

namespace OGLR
{
    template<>
    void VertexArray::addAttrib<glm::vec4>(uint32_t idx, uint32_t stride, uint32_t& offset) const {
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 4, GL_FLOAT, GL_FALSE, (int32_t) stride,
                              reinterpret_cast<const void *>(offset));
        offset += sizeof(glm::vec4);
    }

    template<>
    void VertexArray::addAttrib<glm::vec3>(uint32_t idx, uint32_t stride, uint32_t& offset) const {
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, (int32_t) stride,
                              reinterpret_cast<const void *>(offset));
        offset += sizeof(glm::vec3);
    }

    template<>
    void VertexArray::addAttrib<glm::ivec3>(uint32_t idx, uint32_t stride, uint32_t& offset) const {
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 3, GL_INT, GL_FALSE, (int32_t) stride,
                              reinterpret_cast<const void *>(offset));
        offset += sizeof(glm::ivec3);
    }

    template<>
    void VertexArray::addAttrib<glm::vec2>(uint32_t idx, uint32_t stride, uint32_t& offset) const {
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 2, GL_FLOAT, GL_FALSE, (int32_t) stride,
                              reinterpret_cast<const void *>(offset));
        offset += sizeof(glm::vec2);
    }

    template<>
    void VertexArray::addAttrib<glm::ivec2>(uint32_t idx, uint32_t stride, uint32_t& offset) const {
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 2, GL_INT, GL_FALSE, (int32_t) stride,
                              reinterpret_cast<const void *>(offset));
        offset += sizeof(glm::ivec2);
    }

    template<>
    void VertexArray::addAttrib<glm::uvec2>(uint32_t idx, uint32_t stride, uint32_t& offset) const {
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 2, GL_UNSIGNED_INT, GL_FALSE, (int32_t) stride,
                              reinterpret_cast<const void *>(offset));
        offset += sizeof(glm::uvec2);
    }

    template<>
    void VertexArray::addAttrib<int32_t>(uint32_t idx, uint32_t stride, uint32_t& offset) const {
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 1, GL_INT, GL_FALSE, (int32_t) stride,
                              reinterpret_cast<const void *>(offset));
        offset += sizeof(int32_t);
    }

    template<>
    void VertexArray::addAttrib<uint32_t>(uint32_t idx, uint32_t stride, uint32_t& offset) const {
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 1, GL_UNSIGNED_INT, GL_FALSE, (int32_t) stride,
                              reinterpret_cast<const void *>(offset));
        offset += sizeof(uint32_t);
    }

}
