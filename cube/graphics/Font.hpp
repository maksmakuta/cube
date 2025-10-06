#ifndef CUBE_FONT_HPP
#define CUBE_FONT_HPP
#include <optional>
#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>

namespace cube {

    struct Glyph {
        glm::vec2 uvMin;
        glm::vec2 uvMax;
        int width;
        int height;
        int bearingX;
        int bearingY;
        int advance;
    };

    class Font {
    public:
        Font() = default;
        ~Font();

        static Font fromFile(const std::string& path, float pixelHeight = 32.0f);

        std::optional<Glyph> getGlyph(char c) const;

        glm::uint getTextureID() const { return texID; }
        float getPixelHeight() const { return pixelHeight; }

    private:
        glm::uint texID = 0;
        float pixelHeight = 0.0f;
        std::unordered_map<char, Glyph> glyphs;
    };

}

#endif //CUBE_FONT_HPP