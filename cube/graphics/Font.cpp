#include "Font.hpp"

#include <fstream>
#include <vector>
#include <glm/detail/func_geometric.inl>

#include "TextureBuilder.hpp"

namespace cube {

    Font::Font() = default;
    Font::~Font() = default;

    void Font::load(const std::string& path, int height) {
        constexpr auto size = glm::ivec2{256,256};
        constexpr auto center = size / 2;
        std::vector<unsigned char> data(size.x * size.y,0);

        for (int x = 0; x < size.x; ++x) {
            for (int y = 0; y < size.y; ++y) {
                float d = glm::distance(glm::vec2(x, y), glm::vec2(center));
                d = glm::clamp(d / glm::length(glm::vec2(size) / 2.0f), 0.0f, 1.0f);
                data[y * size.x + x] = static_cast<unsigned char>(d * 255.0f < 128.0f ? 255 : 0);
            }
        }
        m_texture.release();
        m_texture = TextureBuilder()
            .setFormat(TextureFormat::R8)
            .setFilter(TextureFilter::Nearest)
            .setWrap(TextureWrap::ClampEdge)
            .build(data, size);
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
