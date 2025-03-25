#include "cube/graphics/texture.hpp"

#include "cube/graphics/gl/glad.h"

#include <png.h>
#include <iostream>
#include <vector>

namespace cube::graphics {

    void Texture::load(const std::string& path) {
        FILE *fp = fopen(path.c_str(), "rb");
        if (!fp) {
            std::cerr << "Error: Unable to open file " << path << std::endl;
            return;
        }

        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png) {
            fclose(fp);
            std::cerr << "Error: Unable to create PNG read struct" << std::endl;
            return;
        }

        png_infop info = png_create_info_struct(png);
        if (!info) {
            png_destroy_read_struct(&png, nullptr, nullptr);
            fclose(fp);
            std::cerr << "Error: Unable to create PNG info struct" << std::endl;
            return;
        }

        if (_setjmp (*png_set_longjmp_fn(png, longjmp, sizeof (jmp_buf)))) {
            png_destroy_read_struct(&png, &info, nullptr);
            fclose(fp);
            std::cerr << "Error: PNG read error" << std::endl;
            return;
        }

        png_init_io(png, fp);
        png_set_sig_bytes(png, 0);
        png_read_info(png, info);

        m_dimensions.x = png_get_image_width(png, info);
        m_dimensions.y = png_get_image_height(png, info);
        const png_byte color_type = png_get_color_type(png, info);
        const png_byte bit_depth = png_get_bit_depth(png, info);

        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png);
        }
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
            png_set_expand_gray_1_2_4_to_8(png);
        }
        if (png_get_valid(png, info, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(png);
        }
        if (bit_depth == 16) {
            png_set_strip_16(png);
        }
        if (color_type != PNG_COLOR_TYPE_RGBA) {
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
        }

        png_read_update_info(png, info);

        std::vector<png_bytep> row_pointers(m_dimensions.y);
        for (int y = 0; y < m_dimensions.y; y++) {
            row_pointers[y] = static_cast<png_byte *>(malloc(png_get_rowbytes(png, info)));
        }

        png_read_image(png, row_pointers.data());

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(m_dimensions.x), static_cast<int>(m_dimensions.y),
            0, GL_RGBA, GL_UNSIGNED_BYTE, row_pointers[0]);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Free memory
        for (int y = 0; y < m_dimensions.y; y++) {
            free(row_pointers[y]);
        }

        png_destroy_read_struct(&png, &info, nullptr);
        fclose(fp);
    }

    void Texture::clear() {
        glDeleteTextures(1, &m_id);
        m_dimensions.x = m_dimensions.y = 0;
    }

    bool Texture::isOK() const{
        return m_id != 0;
    }

    uint32_t Texture::handle() const{
        return m_id;
    }

    glm::ivec2 Texture::size() const{
        return m_dimensions;
    }

}