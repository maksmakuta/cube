#include "cube/core/Block.hpp"

#include <stdexcept>
#include <vector>

namespace cube {

    const auto BLOCK_INFO = std::vector<BlockInfo>{

    };

    BlockInfo getInfo(const Block& block) {
        if (const auto index = static_cast<int>(block); index < static_cast<int>(Block::COUNT)) {
            return BLOCK_INFO[index];
        }
        throw std::out_of_range("Block index out of range");
    }

}
