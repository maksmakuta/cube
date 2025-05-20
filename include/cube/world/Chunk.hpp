#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "cube/core/Constants.hpp"
#include "cube/models/Block.hpp"

namespace cube {

    class Chunk {
    public:
        Chunk();
        ~Chunk();

        BlockID& at(glm::uint index);
        BlockID& at(const glm::uvec3& pos);

        void set(const glm::uvec3& pos,BlockID);
        [[nodiscard]] BlockID get(const glm::uvec3& pos) const;
        [[nodiscard]] bool is(const glm::ivec3& pos, BlockID i) const;

    private:
        std::vector<BlockID> m_blocks;
    };

    using ChunkPtr = std::shared_ptr<Chunk>;

}

#endif //CHUNK_HPP
