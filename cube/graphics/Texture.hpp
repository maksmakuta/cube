//
// Created by maks on 10/4/25.
//

#ifndef CUBE_TEXTURE_HPP
#define CUBE_TEXTURE_HPP
#include <string>

#include "glad/gl.h"

namespace cube {

    class Texture {
    public:
        explicit Texture(const std::string& path, bool flip = true);
        Texture(int width, int height, GLenum format = GL_RGBA);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        void bind(uint32_t slot = 0) const;

        [[nodiscard]] uint32_t id() const { return m_id; }
        [[nodiscard]] int width() const { return m_width; }
        [[nodiscard]] int height() const { return m_height; }
    private:
        uint32_t m_id = 0;
        int m_width = 0;
        int m_height = 0;
    };

}

#endif //CUBE_TEXTURE_HPP