#include "Texture.hpp"

namespace cube {

    Texture::Texture() = default;
    Texture::~Texture() = default;

    void Texture::bind(const uint32_t slot) const {
        glBindTextureUnit(slot, m_id);
    }

    void Texture::release() {
        if(m_id == 0) return;
        glDeleteTextures(1, &m_id);
        m_id = 0;
        m_size = {0, 0};
    }

    glm::ivec2 Texture::getSize() const {
        return m_size;
    }

    uint32_t Texture::getId() const {
        return m_id;
    }

}
