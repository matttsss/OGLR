#include "Shader.h"

#include <iostream>

namespace OGLR
{
    template<>
    void Shader::setUniform(const std::string &name, const glm::ivec2& val) {
        glUniform2i(getUniformLocation(name), val.x, val.y);
    }

    template<>
    void Shader::setUniform(const std::string &name, const glm::vec2& val) {
        glUniform2f(getUniformLocation(name), val.x, val.y);
    }

    template<>
    void Shader::setUniform(const std::string &name, const glm::ivec3& val) {
        glUniform3i(getUniformLocation(name), val.x, val.y, val.z);
    }

    template<>
    void Shader::setUniform(const std::string &name, const glm::vec3& val) {
        glUniform3f(getUniformLocation(name), val.x, val.y, val.z);
    }

    template<>
    void Shader::setUniform(const std::string &name, const float& val) {
        glUniform1f(getUniformLocation(name), val);
    }

    template<>
    void Shader::setUniform(const std::string &name, const int32_t& val) {
        glUniform1i(getUniformLocation(name), val);
    }

    template<>
    void Shader::setUniform(const std::string &name, const uint32_t& val) {
        glUniform1ui(getUniformLocation(name), val);
    }

    template<>
    void Shader::setUniform(const std::string &name, const glm::mat4& val) {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &val[0][0]);
    }


    GLint Shader::getUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
            std::cout << "Warning: uniform '" << name << "' is not defined... " << std::endl;

        m_UniformLocationCache[name] = location;

        return location;
    }

}
