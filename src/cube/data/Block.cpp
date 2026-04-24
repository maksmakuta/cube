#include <vector>
#include <cube/data/Block.hpp>

namespace cube {

    const auto BLOCK_INFO = std::vector<BlockInfo>{
        {"Air",      {0,0,0,1}},
        {"Grass",    {1,2,3,1}},
        {"Dirt",     {3,3,3,1}},
        {"Stone",    {4,4,4,1}},
        {"Sand",     {5,5,5,1}},
    };

    BlockInfo getBlockInfo(const Block& block) {
        return BLOCK_INFO[block];
    }

    BlockData getBlockData(const Block& block) {
        return getBlockInfo(block).data;
    }

}