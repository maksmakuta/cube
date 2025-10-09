#ifndef CUBE_FONT_HPP
#define CUBE_FONT_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>

#include "Texture.hpp"

namespace cube {

    struct Glyph {
        glm::vec2 size;
        glm::vec2 bearing;
        glm::vec2 uv_min;
        glm::vec2 uv_max;
        int advance;
    };

    class Font {
    public:
        Font();
        ~Font();

        void load(const std::string&, int height = 32);

        [[nodiscard]] std::optional<Glyph> getGlyph(char) const;
        [[nodiscard]] int getSize() const;
        [[nodiscard]] Texture getTexture() const;
    private:
        int m_height = 32;
        Texture m_texture;
        std::unordered_map<char, Glyph> m_glyphs;
    };

}

#endif //CUBE_FONT_HPP