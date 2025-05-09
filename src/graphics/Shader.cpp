#include "cube/graphics/Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/gl.h"

namespace cube {

    namespace internal {

        std::string readAll(const std::string& path) {
            std::ifstream file(path);
            if (!file.is_open()) {
                std::cerr << "Cannot open file: " << path << std::endl;
                return "";
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        GLuint compileShader(const GLenum type, const std::string& source) {
            if (source.empty()) {
                std::cerr << "No sources provided" << std::endl;
                return 0;
            }
            const GLuint shader = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char log[512];
                glGetShaderInfoLog(shader, 512, nullptr, log);
                std::cerr << "Shader compilation error (" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "):\n" << log << '\n';
            }
            return shader;
        }
    }

    void Shader::load(const std::string &vertexPath, const std::string &fragmentPath) {
        const std::string vertexCode = internal::readAll(vertexPath);
        const std::string fragmentCode = internal::readAll(fragmentPath);

        if (vertexCode.empty() || fragmentCode.empty()) {
            return;
        }

        const GLuint vertex = internal::compileShader(GL_VERTEX_SHADER, vertexCode);
        const GLuint fragment = internal::compileShader(GL_FRAGMENT_SHADER, fragmentCode);

        if (vertex == 0 || fragment == 0) {
            return;
        }

        programID = glCreateProgram();
        glAttachShader(programID, vertex);
        glAttachShader(programID, fragment);
        glLinkProgram(programID);

        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cerr << "Shader linking failed:\n" << infoLog << '\n';
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::unload() {
        glDeleteProgram(programID);
        programID = 0;
    }

    void Shader::use() const {
        glUseProgram(programID);
    }

    glm::uint Shader::getID() const {
        return programID;
    }

    void Shader::setBool(const std::string& name, const bool value) const{
        glUniform1i(glGetUniformLocation(programID,name.c_str()),value ? 1 : 0);
    }

    void Shader::setInt(const std::string& name, const int value) const{
        glUniform1i(glGetUniformLocation(programID,name.c_str()),value);
    }

    void Shader::setFloat(const std::string& name, const float value) const{
        glUniform1f(glGetUniformLocation(programID,name.c_str()),value);
    }

    void Shader::setVec2(const std::string& name, const glm::vec2& value) const{
        glUniform2fv(glGetUniformLocation(programID,name.c_str()),1,&value[0]);
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& value) const{
        glUniform3fv(glGetUniformLocation(programID,name.c_str()),1,&value[0]);
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& value) const{
        glUniform4fv(glGetUniformLocation(programID,name.c_str()),1,&value[0]);
    }

    void Shader::setMat3(const std::string& name, const glm::mat3& value) const{
        glUniformMatrix3fv(glGetUniformLocation(programID,name.c_str()),GL_FALSE,1,&value[0][0]);
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& value) const{
        glUniformMatrix4fv(glGetUniformLocation(programID,name.c_str()),GL_FALSE,1,&value[0][0]);
    }

}
