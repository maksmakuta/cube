#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <glm/vec2.hpp>

namespace cube::graphics {

    class Texture {
    public:
        Texture();

        void load();
        void clear();

        [[nodiscard]] bool isOK() const;
        [[nodiscard]] uint32_t handle() const;
        [[nodiscard]] glm::ivec2 size() const;

    private:
        uint32_t id{0};
        glm::uvec2 dimensions{0, 0};
    };

}

#endif //TEXTURE_HPP
