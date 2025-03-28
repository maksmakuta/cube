#include "cube/graphics/font.hpp"

namespace cube::graphics {

    Glyph::Glyph(
        const glm::u8vec2 &size,
        const glm::u8vec2 &offset,
        const uint8_t advance,
        const glm::vec2 &uv_a,
        const glm::vec2 &uv_b
    ) {
        this->data = 0;
        this->data |= size.x & 0x3F;
        this->data |= (size.y & 0x3F) << 6;
        this->data |= (offset.x & 0x3F) << 12;
        this->data |= (offset.y & 0x3F) << 18;
        this->data |= (advance & 0xFF) << 24;
        this->uv = {uv_a, uv_b};
    }

    glm::u8vec2 Glyph::getSize() const{
        return {
            this->data & 0x3F,
            (this->data >> 6 ) & 0x3F
        };
    }

    glm::u8vec2 Glyph::getOffset() const{
        return {
            (this->data >> 12) & 0x3F,
            (this->data >> 18) & 0x3F
        };
    }

    glm::vec2 Glyph::getUVa() const {
        return {uv.x, uv.y};
    }

    glm::vec2 Glyph::getUVb() const{
        return {uv.z, uv.w};
    }

    uint8_t Glyph::getAdvance() const{
        return (this->data >> 24) & 0xFF;
    }

    void Font::load(const std::string &path) {

    }

    void Font::clear() {
        m_glyphs.clear();
        m_texture.clear();
    }

    std::optional<Glyph> Font::get(GlyphID i) const {
        if (m_glyphs.contains(i)) {
            return m_glyphs.at(i);
        }
        return std::nullopt;
    }

    uint32_t Font::getTexture() const {
        return m_texture.handle();
    }

    glm::ivec2 Font::getSize() const {
        return m_texture.size();
    }

}
