#ifndef FONT_HPP
#define FONT_HPP

#include "texture.hpp"

#include <string>
#include <map>
#include <optional>
#include <glm/vec4.hpp>

namespace cube::graphics {

    using GlyphID = unsigned char;

    struct Glyph {
        /**
         * bits: 32                                       0
         *         oooo oooo bbbb bbaa aaaa yyyy yyxx xxxx
         *
         *  x - width -> 6 bit
         *  y - height -> 6 bit
         *  a - offset x -> 6 bit
         *  b - offset y -> 6 bit
         *  o - advance -> 8 bit
         *
         *  data contains size, offsets and advance in compact format
         *
         */
        uint32_t data;
        glm::vec4 uv{};

        Glyph(const glm::u8vec2& size,const glm::u8vec2& offset, uint8_t advance,const glm::vec2& uv_a,const glm::vec2& uv_b);

        [[nodiscard]] glm::u8vec2 getSize() const;
        [[nodiscard]] glm::u8vec2 getOffset() const;
        [[nodiscard]] glm::vec2 getUVa() const;
        [[nodiscard]] glm::vec2 getUVb() const;
        [[nodiscard]] uint8_t getAdvance() const;
    };

    class Font {
    public:
        Font() = default;

        void load(const std::string &path);
        void clear();

        [[nodiscard]] std::optional<Glyph> get(GlyphID) const;
        [[nodiscard]] uint32_t getTexture() const;
        [[nodiscard]] glm::ivec2 getSize() const;

    private:
        std::map<GlyphID,Glyph> m_glyphs;
        Texture m_texture;
    };

}

#endif //FONT_HPP
