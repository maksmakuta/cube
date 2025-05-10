#ifndef FONT_HPP
#define FONT_HPP

#include <glm/vec2.hpp>
#include <optional>
#include <string>
#include <map>

#include "Texture.hpp"

namespace cube {

    enum class Align {
        Start,
        Center,
        End
    };

    namespace Codepoint {
        constexpr auto Latin = glm::ivec2{32,127};
        //you can add more codepoints and add logic to calculate char codes from UTF string
    }

    struct Glyph {
        glm::u8vec2 size;
        glm::i8vec2 offset;
        int advance;
        glm::vec2 uv_a;
        glm::vec2 uv_b;
    };

    class Font {
    public:
        explicit Font(uint8_t size = 24);
        void load(
            const std::string& path,
            const std::initializer_list<glm::ivec2>& codepoints = { Codepoint::Latin }
        );

        void unload();

        [[nodiscard]] std::optional<Glyph> at(int i) const;
        [[nodiscard]] Texture getTexture() const;
        [[nodiscard]] int width(const std::string&) const;
        [[nodiscard]] int height(const std::string&) const;
        [[nodiscard]] int getSize() const;

    private:
        std::map<int, Glyph> m_glyphs;
        Texture m_texture;
        uint8_t m_size;
    };


}

#endif //FONT_HPP
