#ifndef CUBE_SHADER_HPP
#define CUBE_SHADER_HPP

#include <string_view>
#include <unordered_map>
#include <glm/fwd.hpp>

namespace cube {

    class Shader final {
    public:
        explicit Shader(const std::string_view& name);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void use() const;
        void reload();

        void setInt(const std::string_view& name, int value);
        void setFloat(const std::string_view& name, float value);
        void setVec2(const std::string_view& name, const glm::vec2&);
        void setVec3(const std::string_view& name, const glm::vec3&);
        void setVec4(const std::string_view& name, const glm::vec4&);
        void setMat3(const std::string_view& name, const glm::mat3&);
        void setMat4(const std::string_view& name, const glm::mat4&);

    private:
        int getLocation(const std::string_view& name);

        uint32_t m_program{0};
        std::string_view m_name;
        std::unordered_map<std::string_view, int> m_locations;
    };

}

#endif //CUBE_SHADER_HPP
