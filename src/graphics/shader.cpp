#include "cube/graphics/shader.hpp"

#include "cube/graphics/gl/glad.h"

#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

namespace cube::graphics {

    namespace detail {

        uint toShader(const std::string& buff, int type) {
            const auto shaderID = glCreateShader(type);
            const char* src = buff.c_str();
            glShaderSource(shaderID, 1, &src, nullptr);
            glCompileShader(shaderID);
            GLint success;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
            if (!success) {
                GLint logLength;
                glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
                std::string infoLog(logLength, ' ');
                glGetShaderInfoLog(shaderID, logLength, nullptr, &infoLog[0]);
                std::cerr << "Shader compilation error: " << infoLog << std::endl;
            }
            return shaderID;
        }

        std::string toBuffer(const std::string& path) {
            if (std::ifstream file(path); file.is_open()) {
                return {std::istreambuf_iterator(file),std::istreambuf_iterator<char>()};
            }
            return "";
        }

    }

    void Shader::loadMem(const std::string& vert,const std::string& frag) {
        if (vert.empty() || frag.empty()) {
            return;
        }

        const uint v = detail::toShader(vert,GL_VERTEX_SHADER);
        const uint f = detail::toShader(vert,GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        glAttachShader(program, v);
        glAttachShader(program, f);
        glLinkProgram(program);
        glValidateProgram(program);
        glDeleteShader(v);
        glDeleteShader(f);

    }

    void Shader::loadFile(const std::string& vert_path,const std::string& frag_path) {
        loadMem(detail::toBuffer(vert_path),detail::toBuffer(frag_path));
    }

    void Shader::loadFilePair(const std::string &path, const std::string &name) {
        loadFile(path + name + ".vert",path + name + ".frag");
    }

    void Shader::clear() {
        glDeleteProgram(program);
        program = 0;
    }

    bool Shader::isOK() const {
        return program != 0;
    }

    uint32_t Shader::handle() const {
        return program;
    }

    void Shader::setBool(const std::string& name, const bool value) const{
        glUniform1i(glGetUniformLocation(program,name.c_str()),static_cast<int>(value));
    }

    void Shader::setFloat(const std::string& name, const float value) const{
        glUniform1f(glGetUniformLocation(program,name.c_str()),value);
    }

    void Shader::setInt(const std::string& name, const int value) const{
        glUniform1i(glGetUniformLocation(program,name.c_str()),value);
    }

    void Shader::setUInt(const std::string& name, const uint value) const{
        glUniform1ui(glGetUniformLocation(program,name.c_str()),value);
    }

    void Shader::setVec2(const std::string& name,const glm::vec2& value) const{
        glUniform2fv(glGetUniformLocation(program,name.c_str()),1,value_ptr(value));
    }

    void Shader::setVec3(const std::string& name,const glm::vec3& value) const{
        glUniform3fv(glGetUniformLocation(program,name.c_str()),1,value_ptr(value));
    }

    void Shader::setVec4(const std::string& name,const glm::vec4& value) const{
        glUniform4fv(glGetUniformLocation(program,name.c_str()),1,value_ptr(value));
    }

    void Shader::setMat3(const std::string& name,const glm::mat3& value) const{
        glUniformMatrix3fv(glGetUniformLocation(program,name.c_str()),1,GL_FALSE,value_ptr(value));
    }

    void Shader::setMat4(const std::string& name,const glm::mat4& value) const{
        glUniformMatrix4fv(glGetUniformLocation(program,name.c_str()),1,GL_FALSE,value_ptr(value));
    }

}
