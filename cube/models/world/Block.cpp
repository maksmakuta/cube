#include "Block.hpp"

namespace cube {

    std::string toString(const Block& b) {
        switch (b) {
            case Block::Air:
                return "Air";
            case Block::Grass:
                return "Grass";
            case Block::Dirt:
                return "Dirt";
            case Block::Stone:
                return "Stone";
            case Block::Wood:
                return "Wood";
            case Block::Leaves:
                return "Leaves";
            case Block::Water:
                return "Water";
            case Block::Bushes:
                return "Bushes";
            case Block::Flower:
                return "Flower";
            default:
                return "Unknown";
        }
    }
}