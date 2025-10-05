#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_PNG
#include <stb_image.h>
#include <iostream>

namespace cube {

    Texture::Texture(const std::string& path, const bool flip) {
        stbi_set_flip_vertically_on_load(flip);
        int channels;
        unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load texture: " << path << "\n";
            return;
        }

        GLenum format = GL_RGB;
        if (channels == 4) format = GL_RGBA;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
    }

    Texture::Texture(int width, int height, GLenum format)
        : m_width(width), m_height(height) {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
    }

    Texture::~Texture() {
        if (m_id) {
            glDeleteTextures(1, &m_id);
        }
    }

    Texture::Texture(Texture&& other) noexcept {
        m_id = other.m_id;
        m_width = other.m_width;
        m_height = other.m_height;
        other.m_id = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        if (this != &other) {
            if (m_id) glDeleteTextures(1, &m_id);
            m_id = other.m_id;
            m_width = other.m_width;
            m_height = other.m_height;
            other.m_id = 0;
        }
        return *this;
    }

    void Texture::bind(const uint32_t slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

}