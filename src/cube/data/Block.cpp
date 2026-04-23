#include <vector>
#include <cube/data/Block.hpp>

namespace cube {

    const auto BLOCK_INFO = std::vector<BlockInfo>{
        {"Air",      {0,0,0,1}},
        {"Grass",    {0,1,2,1}},
        {"Dirt",     {2,2,2,1}},
        {"Stone",    {3,3,3,1}},
    };

    BlockInfo getBlockInfo(const Block& block) {
        return BLOCK_INFO[block];
    }

    BlockData getBlockData(const Block& block) {
        return getBlockInfo(block).data;
    }

}