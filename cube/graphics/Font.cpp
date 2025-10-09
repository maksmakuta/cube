#include "Font.hpp"

#include <fstream>
#include <vector>

#include "TextureBuilder.hpp"

namespace cube {

    Font::Font() = default;
    Font::~Font() = default;

    void Font::load(const std::string& path, int height) {
        printf("Font::load() in development\n");
    }

    std::optional<Glyph> Font::getGlyph(char c) const {
        if (m_glyphs.contains(c)) {
            return m_glyphs.at(c);
        }
        return std::nullopt;
    }

    int Font::getSize() const {
        return m_height;
    }

    Texture Font::getTexture() const {
        return m_texture;
    }

}
