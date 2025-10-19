#include "TextureArray.hpp"

namespace cube {

    TextureArray::TextureArray() = default;
    TextureArray::~TextureArray() = default;

    void TextureArray::bind(const uint32_t slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    }

    void TextureArray::release() {
        if(m_id == 0) return;
        glDeleteTextures(1, &m_id);
        m_id = 0;
        m_size = {0, 0, 0};
    }

    glm::ivec2 TextureArray::getSize() const {
        return {m_size.x, m_size.y};
    }

    int TextureArray::getLength() const {
        return m_size.z;
    }

    uint32_t TextureArray::getId() const {
        return m_id;
    }

}