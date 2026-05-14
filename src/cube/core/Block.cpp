#include "cube/core/Block.hpp"

#include <stdexcept>
#include <vector>

namespace cube {

    const auto BLOCK_INFO = std::vector<BlockInfo>{
        {  0,  0,  0, 0,  1, 0, false, BlockType::Cube      }, // Air,
        {  3,  0, 19, 1,  1, 0, true,  BlockType::Cube      }, // Grass,
        { 19, 19, 19, 0,  1, 0, false, BlockType::Cube      }, // Dirt,
        { 28, 28, 28, 0,  1, 0, false, BlockType::Cube      }, // Sand,
        { 26, 26, 26, 0,  1, 0, false, BlockType::Cube      }, // RedSand,
        { 22, 22, 22, 0,  1, 0, false, BlockType::Cube      }, // Gravel,
        { 24, 24, 24, 0,  1, 0, false, BlockType::Cube      }, // Mud,
        { 13, 13, 13, 0,  1, 0, false, BlockType::Cube      }, // Clay,
        { 30, 30, 30, 0,  1, 0, false, BlockType::Cube      }, // Stone,
        {  5,  5,  5, 0,  1, 0, false, BlockType::Cube      }, // Andesite,
        {  7,  6,  7, 0,  1, 0, false, BlockType::Cube      }, // Basalt,
        { 12, 12, 12, 0,  1, 0, false, BlockType::Cube      }, // Calcite,
        { 17, 16, 17, 0,  1, 0, false, BlockType::Cube      }, // Deepslate,
        { 18, 18, 18, 0,  1, 0, false, BlockType::Cube      }, // Diorite,
        { 21, 21, 21, 0,  1, 0, false, BlockType::Cube      }, // Granite,
        { 29, 29, 29, 0,  1, 0, false, BlockType::Cube      }, // SandStone,
        { 27, 27, 27, 0,  1, 0, false, BlockType::Cube      }, // RedSandStone,
        { 31, 31, 31, 0,  1, 0, false, BlockType::Cube      }, // Tuff,
        { 23, 23, 23, 0,  1, 0, false, BlockType::Cube      }, // Ice,
        { 11, 11, 11, 0,  1, 0, false, BlockType::Cube      }, // BlueIce,
        { 25, 25, 25, 0,  1, 0, false, BlockType::Cube      }, // PackedIce,
        { 64, 64, 64, 0, 31, 2, true, BlockType::Flowing    }, // Water,
        { 32, 32, 32, 0, 31, 3, false, BlockType::Flowing   }, // Lava,
        {  8,  8,  8, 0,  1, 0, false, BlockType::Cube      }, // Bedrock
    };

    BlockInfo getInfo(const Block& block) {
        const auto index = static_cast<int>(block);
        if (index < static_cast<int>(Block::COUNT)) {
            return BLOCK_INFO[index];
        }
        throw std::out_of_range("Block index out of range");
    }

}
