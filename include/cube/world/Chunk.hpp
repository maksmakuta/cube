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

        BlockID get(const glm::ivec3& pos) const;
        void set(const glm::ivec3& pos,BlockID);

    private:
        std::vector<BlockID> m_blocks;
    };

}

#endif //CHUNK_HPP
