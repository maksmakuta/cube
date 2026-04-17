#include <cube/graphics/Shader.hpp>

#include <fstream>
#include <sstream>
#include <print>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace cube {

    Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
        std::string vCode, fCode;
        try {
            std::ifstream vFile(vertexPath), fFile(fragmentPath);
            std::stringstream vStream, fStream;
            vStream << vFile.rdbuf();
            fStream << fFile.rdbuf();
            vCode = vStream.str();
            fCode = fStream.str();
        } catch (const std::exception& e) {
            std::println(stderr, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: {}", e.what());
        }

        const char* vShaderCode = vCode.c_str();
        const char* fShaderCode = fCode.c_str();

        uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        checkCompileErrors(id, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader::~Shader() {
        glDeleteProgram(id);
    }

    void Shader::checkCompileErrors(uint32_t shader, std::string_view type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::println(stderr, "ERROR::SHADER_COMPILATION_ERROR of type: {}\n{}", type, infoLog);
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::println(stderr, "ERROR::PROGRAM_LINKING_ERROR of type: {}\n{}", type, infoLog);
            }
        }
    }

    void Shader::use() const { glUseProgram(id); }

    void Shader::setMat4(std::string_view name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
    }

}