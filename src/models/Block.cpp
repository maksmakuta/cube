#include "cube/models/Block.hpp"

namespace cube {

    glm::vec3 getColor(const BlockID b) {
        switch (b) {
            case BlockID::Grass:
                return {0.3f, 0.6f, 0.2f};
            case BlockID::Dirt:
                return {0.4f, 0.26f, 0.13f};
            case BlockID::Stone:
                return {0.5f, 0.5f, 0.5f};
            case BlockID::Wood:
                return {0.55f, 0.27f, 0.07f};
            case BlockID::Leaves:
                return {0.2f, 0.5f, 0.2f};
            case BlockID::Bush:
                return {0.25f, 0.55f, 0.25f};
            case BlockID::Flower:
                return {1.0f, 0.3f, 0.6f};
            case BlockID::HardStone:
                return {0.3f, 0.3f, 0.35f};
            default:
                return {1.0f, 1.0f, 1.0f};
        }
    }



}
