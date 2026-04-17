#ifndef CUBE_SHADER_HPP
#define CUBE_SHADER_HPP

#include <glm/glm.hpp>
#include <filesystem>

namespace cube {

    class Shader {
    public:
        Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void use() const;

        void setMat4(std::string_view name, const glm::mat4& mat) const;

    private:
        uint32_t id;
        void checkCompileErrors(uint32_t shader, std::string_view type);
    };

}

#endif //CUBE_SHADER_HPP
