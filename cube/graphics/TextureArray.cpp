#include "TextureArray.hpp"

namespace cube {

    TextureArray::TextureArray() = default;
    TextureArray::~TextureArray() = default;

    void TextureArray::bind(const uint32_t slot) const {
        glBindTextureUnit(slot, m_id);
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