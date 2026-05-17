#ifndef CUBE_SHADER_HPP
#define CUBE_SHADER_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

namespace cube {

    class Shader final {
    public:
        Shader();
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        void use() const;

        bool compile(const std::string& vertexSource, const std::string& fragmentSource);

        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setVec2(const std::string& name, const glm::vec2& value);
        void setVec3(const std::string& name, const glm::vec3& value);
        void setVec4(const std::string& name, const glm::vec4& value);
        void setMat3(const std::string& name, const glm::mat3& value);
        void setMat4(const std::string& name, const glm::mat4& value);

    private:
        int32_t getLocation(const std::string& name);

        uint32_t m_program;
        std::unordered_map<std::string, int32_t> m_uniforms;
    };

}

#endif //CUBE_SHADER_HPP
