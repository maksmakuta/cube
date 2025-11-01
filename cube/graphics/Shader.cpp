#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/gl.h"
#include "utils/Assets.hpp"

namespace cube {

    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    GLuint compile(GLenum type, const std::string& src) {
        GLuint shader = glCreateShader(type);
        const char* code = src.c_str();
        glShaderSource(shader, 1, &code, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, log);
            std::cerr << "Shader compile error:\n" << log << std::endl;
        }
        return shader;
    }

    Shader::Shader() = default;

    Shader::~Shader() {
        if (m_id) glDeleteProgram(m_id);
    }

    void Shader::fromFiles(const std::string& vertPath, const std::string& fragPath) {
        const std::string v = readFile(vertPath);
        const std::string f = readFile(fragPath);

        const GLuint vs = compile(GL_VERTEX_SHADER,   v);
        const GLuint fs = compile(GL_FRAGMENT_SHADER, f);

        m_id = glCreateProgram();
        glAttachShader(m_id, vs);
        glAttachShader(m_id, fs);
        glLinkProgram(m_id);

        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {
            char log[1024];
            glGetProgramInfoLog(m_id, 1024, nullptr, log);
            std::cerr << "Shader link error:\n" << log << std::endl;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    void Shader::fromName(const std::string& name) {
        const auto shaders_paths = Assets::getShader(name);
        fromFiles(
            shaders_paths[0],
            shaders_paths[1]
            );
    }

    uint Shader::getID() const {
        return m_id;
    }

    void Shader::use() const {
        glUseProgram(m_id);
    }

    void Shader::release() {
        if (m_id)
            glDeleteProgram(m_id);
        m_id = 0;
    }

    void Shader::setBool(const std::string& name, const bool value) const {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string& name, const int value) const {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, const float value) const {
        glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
    }

    void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }

    void Shader::setMat3(const std::string& name, const glm::mat3& value) const {
        glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
}
