#ifndef CUBE_TEXTUREARRAY_HPP
#define CUBE_TEXTUREARRAY_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "glad/gl.h"

namespace cube {

    class TextureArray {
    public:
        TextureArray(int width, int height, int layers, GLenum format = GL_RGBA8);
        ~TextureArray();

        static TextureArray fromDir(const std::string& path);
        static TextureArray fromFiles(const std::vector<std::string>& list);

        TextureArray(const TextureArray&) = delete;
        TextureArray& operator=(const TextureArray&) = delete;
        TextureArray(TextureArray&& other) noexcept;
        TextureArray& operator=(TextureArray&& other) noexcept;

        void bind(int slot = 0) const;
        [[nodiscard]] uint32_t id() const { return m_id; }
        [[nodiscard]] int layers() const { return m_layers; }

    private:
        uint32_t m_id = 0;
        int m_width = 0;
        int m_height = 0;
        int m_layers = 0;
    };

}

#endif //CUBE_TEXTUREARRAY_HPP