#include "cube/graphics/TextureAtlas.hpp"

namespace cube {
    TextureAtlas::TextureAtlas() = default;

    TextureAtlas::~TextureAtlas() = default;

    void TextureAtlas::load(const std::string &atlas_file) {
        //TODO(implement this)
    }

    void TextureAtlas::unload() {
        m_tile = {0, 0};
        m_texture.unload();
        m_data.clear();
    }

    Texture TextureAtlas::getTexture() const {
        return m_texture;
    }

    std::optional<AtlasItem> TextureAtlas::getAtlasItem(const BlockID i) const {
        if (m_data.contains(i)) {
            return m_data.at(i);
        }
        return std::nullopt;
    }

    glm::vec2 TextureAtlas::getTileSize() const {
        return m_tile;
    }
}
