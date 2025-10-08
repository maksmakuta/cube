#ifndef CUBE_TEXTURE_HPP
#define CUBE_TEXTURE_HPP

#include <glm/vec2.hpp>

#include "glad/gl.h"

namespace cube {

    class Texture {
        friend class TextureBuilder;
    public:
        Texture();
        ~Texture();

        void bind(uint32_t slot = 0) const;

        [[nodiscard]] glm::ivec2 getSize() const;
        [[nodiscard]] uint32_t getId() const;

    private:
        uint32_t m_id{0};
        glm::ivec2 m_size{0};
    };

}

#endif //CUBE_TEXTURE_HPP