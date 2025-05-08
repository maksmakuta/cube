#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glm/glm.hpp>

namespace cube {

    class Shader {
    public:
        Shader() = default;
        ~Shader() = default;

        void load(const std::string &vertexPath, const std::string &fragmentPath);
        void unload();

        void use() const;
        [[nodiscard]] glm::uint getID() const;

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat3(const std::string& name, const glm::mat3& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

    private:
        glm::uint programID{0};
    };

}

#endif //SHADER_HPP
