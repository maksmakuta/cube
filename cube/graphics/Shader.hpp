#ifndef CUBE_SHADER_HPP
#define CUBE_SHADER_HPP

#include <string>
#include <glm/glm.hpp>

namespace cube {

    class Shader {
    public:
        Shader();
        ~Shader();

        void fromFiles(const std::string&,const std::string&);
        void fromName(const std::string&);

        [[nodiscard]] uint getID() const;

        void use() const;
        void release();

        void setBool (const std::string&, bool value) const;
        void setInt  (const std::string&, int value) const;
        void setFloat(const std::string&, float value) const;
        void setVec2 (const std::string&, const glm::vec2& value) const;
        void setVec3 (const std::string&, const glm::vec3& value) const;
        void setVec4 (const std::string&, const glm::vec4& value) const;
        void setMat3 (const std::string&, const glm::mat3& value) const;
        void setMat4 (const std::string&, const glm::mat4& value) const;
    private:
        uint m_id{0};
    };

}

#endif //CUBE_SHADER_HPP