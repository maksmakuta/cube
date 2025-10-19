#include "Block.hpp"

#include <unordered_map>

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

    static auto METADATA = std::unordered_map<Block, BlockMetadata>{
            { Block::Air    , BlockMetadata{-1,-1,-1} },
            { Block::Grass  , BlockMetadata{ 2, 0, 1} },
            { Block::Dirt   , BlockMetadata{ 1, 1, 1} },
            { Block::Stone  , BlockMetadata{ 3, 3, 3} },
            { Block::Wood   , BlockMetadata{ 5, 4, 5} },
            { Block::Leaves , BlockMetadata{ 6, 6, 6} },
            { Block::Bushes , BlockMetadata{ 8, 8, 8} },
            { Block::Flower , BlockMetadata{ 9, 9, 9} },
            { Block::Water  , BlockMetadata{ 7, 7, 7} },
    };

    BlockMetadata getMetadata(const Block& b) {
        return METADATA[b];
    }
}
