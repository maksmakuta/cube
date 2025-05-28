#include "cube/models/Block.hpp"

#include <glm/common.hpp>
#include <glm/vec2.hpp>

namespace cube {
    const auto texture_size = glm::vec2{640};
    const auto tile_size = glm::ivec2{128};

    glm::vec4 getUV(const glm::ivec2 &pos) {
        const auto a = pos * tile_size;
        const auto b = a + tile_size;
        return {glm::floor<2, float, glm::highp>(a) / texture_size, glm::floor<2, float, glm::highp>(b) / texture_size};
    }

    glm::vec4 getTile(const BlockID i, const Face f) {
        switch (i) {
            case BlockID::Grass:
                if (f == Face::Top) {
                    return getUV({0, 0});
                }
                if (f == Face::Bottom) {
                    return getUV({2, 0});
                }
                return getUV({1, 0});
            case BlockID::Dirt:
                return getUV({2, 0});
            case BlockID::Sand:
                return getUV({0, 1});
            case BlockID::Stone:
                return getUV({0, 3});
            case BlockID::Graystone:
                return getUV({0, 4});
            case BlockID::WoodDark:
                if (f == Face::Top || f == Face::Bottom) {
                    return getUV({1, 2});
                }
                return getUV({1, 1});
            case BlockID::WoodLight:
                if (f == Face::Top || f == Face::Bottom) {
                    return getUV({1, 4});
                }
                return getUV({1, 3});
            case BlockID::LeavesGreen:
                return getUV({4, 2});
            case BlockID::LeavesOrange:
                return getUV({0, 3});
            case BlockID::MushroomRed:
                return getUV({2, 3});
            case BlockID::MushroomBrown:
                return getUV({1, 3});
            case BlockID::Cactus:
                if (f == Face::Top) {
                    return getUV({0, 2});
                }
                if (f == Face::Bottom) {
                    return getUV({2, 2});
                }
                return getUV({1, 2});
            case BlockID::WeedDry:
                return getUV({3, 3});
            case BlockID::Weed:
                return getUV({0, 4});
            case BlockID::Water:
                return getUV({3, 2});
            default:
                return glm::vec4{0};
        }
    }
}
