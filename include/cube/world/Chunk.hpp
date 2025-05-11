#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <vector>

#include "cube/core/Constants.hpp"
#include "cube/models/Block.hpp"

namespace cube {

    class Chunk {
    public:
        Chunk();
        ~Chunk();

        BlockID& at(const glm::ivec3& pos);
        [[nodiscard]] bool is(const glm::ivec3& pos, BlockID) const;

    private:
        std::vector<BlockID> m_blocks{CHUNK_SIZE,BlockID::Air};
    };

}

#endif //CHUNK_HPP
