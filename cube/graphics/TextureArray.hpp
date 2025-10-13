#ifndef CUBE_TEXTUREARRAY_HPP
#define CUBE_TEXTUREARRAY_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "glad/gl.h"

namespace cube {

    class TextureArray {
        friend class TextureBuilder;
    public:
        TextureArray();
        ~TextureArray();

        void bind(uint32_t slot = 0) const;
        void release();

        [[nodiscard]] glm::ivec2 getSize() const;
        [[nodiscard]] int getLength() const;
        [[nodiscard]] uint32_t getId() const;

    private:
        uint32_t m_id = 0;
        glm::ivec3 m_size = {0, 0, 0};
    };

}

#endif //CUBE_TEXTUREARRAY_HPP