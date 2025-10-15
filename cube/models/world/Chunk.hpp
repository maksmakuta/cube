#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <vector>
#include <glm/vec3.hpp>

#include "Block.hpp"

namespace cube {

    using BlockPos = glm::ivec3;

    constexpr auto CHUNK_SIZE = glm::ivec3(16,128,16);

    class Chunk {
    public:
        Chunk();
        ~Chunk();

        void setBlock(const BlockPos& pos, Block);
        Block getBlock(const BlockPos&) const;
    private:
        std::vector<Block> m_blocks;
    };

}



#endif //CUBE_CHUNK_HPP