#ifndef SHADER_HPP
#define SHADER_HPP

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

namespace cube::graphics {

    class Shader {
    public:
        Shader() = default;

        void loadMem(const std::string& vert,const std::string& frag);
        void loadFile(const std::string& vert_path,const std::string& frag_path);
        void loadFilePair(const std::string &path, const std::string &name);
        void clear();

        [[nodiscard]] bool isOK() const;
        [[nodiscard]] uint32_t handle() const;

        void setBool(const std::string& name, bool value) const;
        void setFloat(const std::string& name, float value) const;
        void setInt(const std::string& name, int value) const;
        void setUInt(const std::string& name, uint value) const;
        void setVec2(const std::string& name,const glm::vec2& value) const;
        void setVec3(const std::string& name,const glm::vec3& value) const;
        void setVec4(const std::string& name,const glm::vec4& value) const;
        void setMat3(const std::string& name,const glm::mat3& value) const;
        void setMat4(const std::string& name,const glm::mat4& value) const;
    private:
        uint32_t m_program{0};
    };
}

#endif //SHADER_HPP
