#ifndef TEXTURATLAS_HPP
#define TEXTURATLAS_HPP

#include <optional>
#include <string>
#include <unordered_map>

#include "cube/graphics/Texture.hpp"
#include "cube/models/Block.hpp"

namespace cube {
    enum class AtlasFace {
        Top,
        Side,
        Bottom
    };

    struct AtlasItem {
    };

    class TextureAtlas {
    public:
        TextureAtlas();

        ~TextureAtlas();

        void load(const std::string &atlas_file);

        void unload();

        Texture getTexture() const;

        std::optional<AtlasItem> getAtlasItem(BlockID) const;

        glm::vec2 getTileSize() const;

    private:
        std::unordered_map<BlockID, AtlasItem> m_data;
        Texture m_texture;
        glm::vec2 m_tile{0.f};
    };
}

#endif //TEXTURATLAS_HPP
