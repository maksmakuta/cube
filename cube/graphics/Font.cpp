#include "Font.hpp"

#include <vector>
#include <ft2build.h>
#include <stdexcept>

#include FT_FREETYPE_H

#include "TextureBuilder.hpp"

namespace cube {

    Font::Font() = default;
    Font::~Font() = default;

    void Font::load(const std::string& path, int height) {
        m_height = height;

        FT_Library ft;
        if (FT_Init_FreeType(&ft)) throw std::runtime_error("Failed to init FreeType");

        FT_Face face;
        if (FT_New_Face(ft, path.c_str(), 0, &face)) throw std::runtime_error("Failed to load font");

        FT_Set_Pixel_Sizes(face, 0, height);

        // Determine atlas size (simple: 16x16 chars)
        constexpr int ATLAS_COLS = 16;
        constexpr int ATLAS_ROWS = 16;
        int atlas_width = ATLAS_COLS * height;
        int atlas_height = ATLAS_ROWS * height;
        std::vector<uint8_t> pixels(atlas_width * atlas_height, 0);

        int x = 0, y = 0;
        for (unsigned char c = 32; c < 128; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                continue;

            FT_GlyphSlot g = face->glyph;

            // Copy bitmap to atlas
            for (int i = 0; i < g->bitmap.width; ++i) {
                for (int j = 0; j < g->bitmap.rows; ++j) {
                    int atlas_index = (y + j) * atlas_width + (x + i);
                    pixels[atlas_index] = g->bitmap.buffer[j * g->bitmap.width + i];
                }
            }

            Glyph glyph{};
            glyph.size = { (float)g->bitmap.width, (float)g->bitmap.rows };
            glyph.bearing = { (float)g->bitmap_left, (float)g->bitmap_top };
            glyph.advance = static_cast<int>(g->advance.x >> 6);

            glyph.uv_min = { (float)x / atlas_width, (float)y / atlas_height };
            glyph.uv_max = { (float)(x + g->bitmap.width) / atlas_width, (float)(y + g->bitmap.rows) / atlas_height };

            m_glyphs[c] = glyph;

            x += height;
            if (x + height > atlas_width) { x = 0; y += height; }
        }

        // Build OpenGL texture
        m_texture = TextureBuilder().setSize({ atlas_width, atlas_height })
                                   .setFormat(TextureFormat::R8)
                                   .build(pixels, { atlas_width, atlas_height });

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void Font::unload() {
        m_texture.release();
        m_glyphs.clear();
    }

    glm::vec2 Font::measure(const std::string &text) const {
        glm::vec2 size(0.0f);
        for (const auto& c : text) {
            if (const auto glyph = getGlyph(c)) {
                size.x += static_cast<float>(glyph->advance);
                size.y = std::max(size.y, glyph->size.y - glyph->bearing.y);
            }
        }
        return size;
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
