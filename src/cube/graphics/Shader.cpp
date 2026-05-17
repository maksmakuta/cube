#include "cube/graphics/Shader.hpp"

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace cube {

    Shader::Shader() : m_program(0) {}

    Shader::~Shader() {
        if (m_program != 0) {
            glDeleteProgram(m_program);
        }
    }

    Shader::Shader(Shader&& other) noexcept
        : m_program(other.m_program), m_uniforms(std::move(other.m_uniforms)) {
        other.m_program = 0;
    }

    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this != &other) {
            if (m_program != 0) {
                glDeleteProgram(m_program);
            }
            m_program = other.m_program;
            m_uniforms = std::move(other.m_uniforms);
            other.m_program = 0;
        }
        return *this;
    }

    void Shader::use() const {
        glUseProgram(m_program);
    }

    int32_t Shader::getLocation(const std::string& name) {
        auto it = m_uniforms.find(name);
        if (it != m_uniforms.end()) {
            return it->second;
        }

        int32_t location = glGetUniformLocation(m_program, name.c_str());
        m_uniforms[name] = location;
        return location;
    }

    bool Shader::compile(const std::string& vertexSource, const std::string& fragmentSource) {
        const char* vsCode = vertexSource.c_str();
        const char* fsCode = fragmentSource.c_str();

        const uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vsCode, nullptr);
        glCompileShader(vertex);

        int success;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }

        const uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fsCode, nullptr);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }

        m_program = glCreateProgram();
        glAttachShader(m_program, vertex);
        glAttachShader(m_program, fragment);
        glLinkProgram(m_program);

        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            return false;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return true;
    }

    void Shader::setInt(const std::string& name, int value) {
        glUniform1i(getLocation(name), value);
    }

    void Shader::setFloat(const std::string& name, float value) {
        glUniform1f(getLocation(name), value);
    }

    void Shader::setVec2(const std::string& name, const glm::vec2& value) {
        glUniform2fv(getLocation(name), 1, glm::value_ptr(value));
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& value) {
        glUniform3fv(getLocation(name), 1, glm::value_ptr(value));
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& value) {
        glUniform4fv(getLocation(name), 1, glm::value_ptr(value));
    }

    void Shader::setMat3(const std::string& name, const glm::mat3& value) {
        glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& value) {
        glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

}