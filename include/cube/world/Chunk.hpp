#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "cube/models/Block.hpp"

namespace cube {

    class Chunk {
    public:
        Chunk();
        ~Chunk();

        void setBlock(int x, int y, int z, BlockID b);
        void setBlock(const glm::ivec3 &pos, BlockID b);
        [[nodiscard]] BlockID getBlock(int x, int y, int z) const;
        [[nodiscard]] BlockID getBlock(const glm::ivec3 &pos) const;

        void fill(const glm::ivec3 &a, const glm::ivec3 &b, BlockID blk);
    private:
        std::vector<BlockID> m_data;
    };

    using ChunkPtr = std::shared_ptr<Chunk>;
}

#endif //CHUNK_HPP
