#include <vector>
#include <cube/data/Block.hpp>

namespace cube {

    const auto BLOCK_INFO = std::vector<BlockInfo>{
        {"Air",      {0,0,0,0}},
        {"Grass",    {0,0,0,0}},
        {"Dirt",     {0,0,0,0}},
        {"Stone",    {0,0,0,0}},
    };

    BlockInfo getBlockInfo(const Block& block) {
        return BLOCK_INFO[block];
    }

    BlockData getBlockData(const Block& block) {
        return getBlockInfo(block).data;
    }

}