#include "cube/graphics/Shader.hpp"

#include <expected>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "assets_dir.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "cube/utils/Log.hpp"

namespace cube {

    Shader::Shader(const std::string_view& name) : m_name(name) {
        reload();
    }

    Shader::~Shader() {
        if (m_program != 0)
            glDeleteProgram(m_program);
    }

    void Shader::use() const {
        glUseProgram(m_program);
    }

    void printShaderLog(const uint32_t shader, const uint32_t type) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetShaderInfoLog(shader, length, nullptr, message.data());

        error("--- {} SHADER COMPILE ERROR ---", type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
        std::cout << message.data() << std::endl;
    }

    void printProgramLog(const uint32_t program) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetProgramInfoLog(program, length, nullptr, message.data());

        error("--- SHADER LINKING ERROR ---");
        std::cout << message.data() << std::endl;
    }

    std::expected<std::string, std::string> readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return std::unexpected(std::format("Failed to open: {}", path));
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    uint32_t compileShader(const uint32_t type, const std::string& source) {
        const uint32_t shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            printShaderLog(shader, type);
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    void Shader::reload() {
        const auto vertexResult = readFile(std::format("{}shaders/{}.vert",ASSETS_PATH, m_name));
        const auto fragmentResult = readFile(std::format("{}shaders/{}.frag",ASSETS_PATH, m_name)));

        if (!vertexResult || !fragmentResult) {
            std::cerr << std::format("FILE_ERROR: {}\n", !vertexResult ? vertexResult.error() : fragmentResult.error());
            return;
        }

        const uint32_t vShader = compileShader(GL_VERTEX_SHADER, *vertexResult);
        const uint32_t fShader = compileShader(GL_FRAGMENT_SHADER, *fragmentResult);

        if (vShader == 0 || fShader == 0) {
            glDeleteShader(vShader);
            glDeleteShader(fShader);
            return;
        }

        const uint32_t newProgram = glCreateProgram();
        glAttachShader(newProgram, vShader);
        glAttachShader(newProgram, fShader);
        glLinkProgram(newProgram);

        int success;
        glGetProgramiv(newProgram, GL_LINK_STATUS, &success);
        if (!success) {
            printProgramLog(newProgram);
            glDeleteShader(vShader);
            glDeleteShader(fShader);
            glDeleteProgram(newProgram);
            return;
        }

        glDeleteShader(vShader);
        glDeleteShader(fShader);

        if (m_program != 0) {
            glDeleteProgram(m_program);
            std::cout << std::format("SHADER_SUCCESS: {} reloaded.\n", m_name);
        }else {
            std::cout << std::format("SHADER_SUCCESS: {} loaded.\n", m_name);
        }
        m_program = newProgram;
        m_locations.clear();

    }

    void Shader::setInt(const std::string_view& name, const int value) {
        glProgramUniform1i(m_program, getLocation(name), value);
    }

    void Shader::setFloat(const std::string_view& name, const float value) {
        glProgramUniform1f(m_program, getLocation(name), value);
    }

    void Shader::setVec2(const std::string_view& name, const glm::vec2& value) {
        glProgramUniform2fv(m_program, getLocation(name), 1, glm::value_ptr(value));
    }

    void Shader::setVec3(const std::string_view& name, const glm::vec3& value) {
        glProgramUniform3fv(m_program, getLocation(name), 1, glm::value_ptr(value));
    }

    void Shader::setVec4(const std::string_view& name, const glm::vec4& value) {
        glProgramUniform4fv(m_program, getLocation(name), 1, glm::value_ptr(value));
    }

    void Shader::setMat3(const std::string_view& name, const glm::mat3& value) {
        glProgramUniformMatrix3fv(m_program, getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::setMat4(const std::string_view& name, const glm::mat4& value ) {
        glProgramUniformMatrix4fv(m_program, getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    int Shader::getLocation(const std::string_view& name) {
        if (const auto it = m_locations.find(name); it != m_locations.end()) {
            return it->second;
        }
        const auto location = glGetUniformLocation(m_program, name.data());
        m_locations[name] = location;
        return location;
    }

}

