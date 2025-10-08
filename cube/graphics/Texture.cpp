#include "Texture.hpp"

namespace cube {

    Texture::Texture() = default;
    Texture::~Texture() = default;

    void Texture::bind(const uint32_t slot) const {
        glBindTextureUnit(slot, m_id);
    }

    glm::ivec2 Texture::getSize() const {
        return m_size;
    }

    uint32_t Texture::getId() const {
        return m_id;
    }

}
