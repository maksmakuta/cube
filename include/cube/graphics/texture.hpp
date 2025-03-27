#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <string>
#include <glm/vec2.hpp>

namespace cube::graphics {

    class Texture {
    public:
        Texture() = default;

        void load(const std::string& path);
        void load(uint32_t id, const glm::ivec2& size);
        void clear();

        [[nodiscard]] bool isOK() const;
        [[nodiscard]] uint32_t handle() const;
        [[nodiscard]] glm::ivec2 size() const;

    private:
        uint32_t m_id{0};
        glm::uvec2 m_dimensions{0, 0};
    };

}

#endif //TEXTURE_HPP
