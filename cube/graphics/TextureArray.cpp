#include "TextureArray.hpp"

#include <stb_image.h>

#include <algorithm>
#include <iostream>
#include <filesystem>

namespace cube {
    TextureArray::TextureArray(const int width, const int height, const int layers, GLenum format)
        : m_width(width), m_height(height), m_layers(layers)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

        glTexImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            format,
            width,
            height,
            layers,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    TextureArray::~TextureArray() {
        if (m_id) {
            glDeleteTextures(1, &m_id);
        }
    }

    TextureArray::TextureArray(TextureArray&& other) noexcept {
        m_id = other.m_id;
        m_width = other.m_width;
        m_height = other.m_height;
        m_layers = other.m_layers;
        other.m_id = 0;
    }

    TextureArray& TextureArray::operator=(TextureArray&& other) noexcept {
        if (this != &other) {
            if (m_id) glDeleteTextures(1, &m_id);
            m_id = other.m_id;
            m_width = other.m_width;
            m_height = other.m_height;
            m_layers = other.m_layers;
            other.m_id = 0;
        }
        return *this;
    }

    void TextureArray::bind(int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    }

    TextureArray TextureArray::fromDir(const std::string& path) {
        namespace fs = std::filesystem;
        std::vector<std::string> files;

        for (auto& entry : fs::directory_iterator(path)) {
            if (!entry.is_regular_file()) continue;
            auto ext = entry.path().extension().string();

            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" ||
                ext == ".bmp" || ext == ".tga")
            {
                files.push_back(entry.path().string());
            }
        }

        std::sort(files.begin(), files.end());

        if (files.empty()) {
            std::cerr << "TextureArray::fromDir: no textures in " << path << "\n";
            return {0, 0, 0};
        }

        return fromFiles(files);
    }

    TextureArray TextureArray::fromFiles(const std::vector<std::string>& files) {
        int w, h, ch;
        stbi_uc* first = stbi_load(files[0].c_str(), &w, &h, &ch, STBI_rgb_alpha);
        if (!first) {
            std::cerr << "Failed to load texture: " << files[0] << "\n";
            return {0, 0, 0};
        }
        stbi_image_free(first);

        TextureArray arr(w, h, files.size());

        for (int i = 0; i < (int)files.size(); i++) {
            stbi_uc* data = stbi_load(files[i].c_str(), &w, &h, &ch, STBI_rgb_alpha);
            if (!data) {
                std::cerr << "Failed to load texture: " << files[i] << "\n";
                continue;
            }

            glTextureSubImage3D(arr.id(), 0, 0, 0, i, w, h, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }

        return arr;
    }

}