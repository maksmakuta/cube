#include "Block.hpp"

#include <unordered_map>
#include <vector>

namespace cube {

    const auto BLOCK_INFOS = std::unordered_map<Block,BlockInfo>{
        {Block::Air     ,{BlockType::Empty ,-1,-1,-1}},
        {Block::Grass   ,{BlockType::Fill  , 2, 0, 1}},
        {Block::Dirt    ,{BlockType::Fill  , 1, 1, 1}},
        {Block::Stone   ,{BlockType::Fill  , 3, 3, 3}},
        {Block::Wood    ,{BlockType::Fill  , 5, 4, 5}},
        {Block::Leaves  ,{BlockType::Fill  , 6, 6, 6}},
        {Block::Water   ,{BlockType::Fill  , 7, 7, 7}},
        {Block::Bush    ,{BlockType::Sharp ,-1, 8,-1}},
        {Block::Flower  ,{BlockType::Cross ,-1, 9,-1}},
    };

    BlockInfo getBlockInfo(const Block& block) {
        return BLOCK_INFOS.at(block);
    }

}
