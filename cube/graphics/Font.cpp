#include "Font.hpp"

#include "glad/gl.h"

#include <stb_truetype.h>
#include <vector>
#include <fstream>
#include <iostream>

namespace cube {

    Font::~Font() {
        if (texID) {
            glDeleteTextures(1, &texID);
        }
        glyphs.clear();
    }

    Font Font::fromFile(const std::string& path, float pixelHeight) {
        Font font;
        font.pixelHeight = pixelHeight;

        std::ifstream ifs(path, std::ios::binary | std::ios::ate);
        if (!ifs) {
            std::cerr << "Failed to open font: " << path << std::endl;
            return font;
        }

        auto size = ifs.tellg();
        std::vector<unsigned char> buffer(size);
        ifs.seekg(0, std::ios::beg);
        ifs.read(reinterpret_cast<char*>(buffer.data()), size);

        stbtt_fontinfo info;
        if (!stbtt_InitFont(&info, buffer.data(), stbtt_GetFontOffsetForIndex(buffer.data(), 0))) {
            std::cerr << "Failed to init stb font: " << path << std::endl;
            return font;
        }

        float scale = stbtt_ScaleForPixelHeight(&info, pixelHeight);

        const int texWidth  = 512;
        const int texHeight = 512;
        std::vector<unsigned char> bitmap(texWidth * texHeight, 0);

        int xOffset = 0;
        int yOffset = 0;
        int rowHeight = 0;

        for (int c = 32; c < 127; ++c) {
            int w, h, xoff, yoff;
            unsigned char* glyph = stbtt_GetCodepointBitmap(&info, 0, scale, c, &w, &h, &xoff, &yoff);

            if (!glyph) continue;

            if (xOffset + w >= texWidth) {
                xOffset = 0;
                yOffset += rowHeight;
                rowHeight = 0;
            }

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    bitmap[(yOffset + y) * texWidth + (xOffset + x)] = glyph[y * w + x];
                }
            }

            stbtt_FreeBitmap(glyph, nullptr);

            int ax, lsb;
            stbtt_GetCodepointHMetrics(&info, c, &ax, &lsb);

            int bx1, by1, bx2, by2;
            stbtt_GetCodepointBitmapBox(&info, c, scale, scale, &bx1, &by1, &bx2, &by2);

            Glyph g{};
            g.uvMin = glm::vec2(
                static_cast<float>(xOffset) / static_cast<float>(texWidth),
                static_cast<float>(yOffset) / static_cast<float>(texHeight)
            );
            g.uvMax = glm::vec2(
                static_cast<float>(xOffset + w) / static_cast<float>(texWidth),
                static_cast<float>(yOffset + h) / static_cast<float>(texHeight)
            );
            g.width    = w;
            g.height   = h;
            g.bearingX = bx1;
            g.bearingY = by1;
            g.advance  = static_cast<int>(static_cast<float>(ax) * scale);

            font.glyphs[static_cast<char>(c)] = g;

            xOffset += w + 1;
            if (h > rowHeight) rowHeight = h;
        }


        glCreateTextures(GL_TEXTURE_2D, 1, &font.texID);
        glTextureStorage2D(font.texID, 1, GL_R8, texWidth, texHeight);
        glTextureSubImage2D(font.texID, 0, 0, 0, texWidth, texHeight, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());

        glTextureParameteri(font.texID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(font.texID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(font.texID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(font.texID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        return font;
    }

    std::optional<Glyph> Font::getGlyph(const char c) const {
        const auto it = glyphs.find(c);
        if (it == glyphs.end()) return std::nullopt;
        return it->second;
    }

}
