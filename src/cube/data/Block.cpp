#include <cube/data/Block.hpp>

#include <array>

namespace cube {

    static constexpr std::array<BlockInfo, COUNT> BLOCK_REGISTRY = {{
        {"Air",   {0, 0, 0, 0}},
        {"Grass", {1, 2, 3, 1}},
        {"Dirt",  {3, 3, 3, 1}},
        {"Stone", {4, 4, 4, 1}},
        {"Sand",  {5, 5, 5, 1}}
    }};

    const BlockInfo& getBlockInfo(Block block) {
        return BLOCK_REGISTRY[block];
    }

    BlockData getBlockData(Block block) {
        return BLOCK_REGISTRY[block].data;
    }

}