#include "TextureBuilder.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_PNG
#include <filesystem>
#include <iostream>
#include <stb_image.h>
#include <stdexcept>

namespace cube {

    TextureBuilder::TextureBuilder() = default;
    TextureBuilder::~TextureBuilder() = default;

    TextureBuilder& TextureBuilder::setSize(const glm::ivec2& size) {
        m_size = {size.x, size.y, 1};
        return *this;
    }

    TextureBuilder& TextureBuilder::setSize(const glm::ivec3& size) {
        m_size = size;
        return *this;
    }

    TextureBuilder& TextureBuilder::setFormat(const TextureFormat& fmt) {
        m_format = fmt;
        return *this;
    }

    TextureBuilder& TextureBuilder::setFilter(const TextureFilter& filter) {
        m_minFilter = m_magFilter = filter;
        return *this;
    }

    TextureBuilder& TextureBuilder::setFilter(const TextureFilter& min, const TextureFilter& mag) {
        m_minFilter = min;
        m_magFilter = mag;
        return *this;
    }

    TextureBuilder& TextureBuilder::setWrap(const TextureWrap& wrap) {
        m_wrapS = m_wrapT = wrap;
        return *this;
    }

    TextureBuilder& TextureBuilder::setWrap(const TextureWrap& s, const TextureWrap& t) {
        m_wrapS = s;
        m_wrapT = t;
        return *this;
    }

    TextureBuilder& TextureBuilder::setFlipY(const bool flip) {
        m_flip = flip;
        return *this;
    }

    TextureArray TextureBuilder::buildArray(const std::string& dir_path) {
        namespace fs = std::filesystem;
        std::vector<std::string> files;
        for (auto& e : fs::directory_iterator(dir_path)) {
            if (e.is_regular_file())
                files.push_back(e.path().string());
        }
        return buildArray(files);
    }

    TextureArray TextureBuilder::buildArray(const std::vector<std::string> &items) {
        if (items.empty()) throw std::runtime_error("TextureArray: empty file list.");

        stbi_set_flip_vertically_on_load(m_flip);
        int w, h, ch;
        unsigned char* first = stbi_load(items[0].c_str(), &w, &h, &ch, 0);
        if (!first) throw std::runtime_error("Failed to load first texture: " + items[0]);
        stbi_image_free(first);

        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texID);
        applyParams(GL_TEXTURE_2D_ARRAY);

        const GLenum format = ch == 4 ? GL_RGBA : GL_RGB;
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, static_cast<GLint>(m_format),
                     w, h, static_cast<GLsizei>(items.size()), 0, format, GL_UNSIGNED_BYTE, nullptr);

        for (size_t i = 0; i < items.size(); ++i) {
            int tw, th, tch;
            unsigned char* data = stbi_load(items[i].c_str(), &tw, &th, &tch, ch);
            if (!data) {
                std::cerr << "Warning: skip texture " << items[i] << "\n";
                m_size.z--;
                continue;
            }
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, static_cast<GLint>(i),
                            w, h, 1, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }

        TextureArray arr;
        arr.m_id = texID;
        arr.m_size = {w, h,m_size.z};
        return arr;
    }

    TextureArray TextureBuilder::buildArray() {
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texID);
        applyParams(GL_TEXTURE_2D_ARRAY);

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, static_cast<GLint>(m_format),
                     m_size.x, m_size.y, m_size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        TextureArray arr;
        arr.m_id = texID;
        arr.m_size = m_size;
        return arr;
    }

    Texture TextureBuilder::build(const std::vector<uint8_t>& data, const glm::ivec2& size) {
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        applyParams(GL_TEXTURE_2D);

        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_format),
                     size.x, size.y, 0, static_cast<GLenum>(m_format), GL_UNSIGNED_BYTE, data.data());

        Texture texture;
        texture.m_id = texID;
        texture.m_size = size;
        return texture;
    }

    Texture TextureBuilder::build(const std::string& file_path) {
        stbi_set_flip_vertically_on_load(m_flip);

        int w, h, ch;
        unsigned char* data = stbi_load(file_path.c_str(), &w, &h, &ch, 0);
        if (!data) {
            throw std::runtime_error("Failed to load texture: " + file_path);
        }

        const GLenum format = ch == 4 ? GL_RGBA : GL_RGB;
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        applyParams(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_format), w, h, 0, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        Texture texture;
        texture.m_id = texID;
        texture.m_size = {w, h};
        return texture;
    }

    Texture TextureBuilder::build() {
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        applyParams(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_format), m_size.x, m_size.y, 0, static_cast<GLenum>(m_format), GL_UNSIGNED_BYTE, nullptr);

        Texture texture;
        texture.m_id = texID;
        texture.m_size = {m_size.x, m_size.y};
        return texture;
    }

    void TextureBuilder::applyParams(const GLenum target) const {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_minFilter));
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_magFilter));
        glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_wrapS));
        glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_wrapT));
    }

}
