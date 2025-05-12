#include "cube/models/Block.hpp"

#include <glm/vec2.hpp>

#include "cube/core/Constants.hpp"

namespace cube {
    glm::vec4 toUV(const glm::vec2& pos) {
        const auto a = pos * TILE_SIZE;
        const auto b = (pos + 1.f)  * TILE_SIZE;
        return glm::vec4{a / ATLAS_SIZE, b / ATLAS_SIZE};
    }

    glm::vec4 getTile(const BlockID b, const int face) {
        switch (b) {
            case BlockID::Grass: {
                if (face == 2) {
                    return toUV({2, 3});
                }
                if (face == 3) {
                    return toUV({1, 2});
                }
                return toUV({1, 3});
            }
            case BlockID::Dirt:
                return toUV({1, 2});
            case BlockID::Stone:
                return toUV({7, 0});
            case BlockID::Wood:{
                if (face == 2 || face == 3) {
                    return toUV({5, 0});
                }
                return toUV({4, 0});
            }
            case BlockID::Leaves:
                return toUV({3, 0});
            case BlockID::HardStone:
                return toUV({0, 0});
            default:
                return glm::vec4{0};
        }
    }

}
