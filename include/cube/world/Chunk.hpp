#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <vector>

#include "cube/core/Constants.hpp"
#include "cube/models/Block.hpp"

namespace cube {

    class Chunk {
    public:
        explicit Chunk(const glm::ivec2& offset = {0,0});
        ~Chunk();

        BlockID& at(glm::uint index);
        BlockID& at(const glm::uvec3& pos);

        [[nodiscard]] BlockID get(const glm::uvec3& pos) const;
        [[nodiscard]] bool is(const glm::ivec3& pos, BlockID i) const;
        [[nodiscard]] glm::ivec2 getOffset() const;

    private:
        std::vector<BlockID> m_blocks{CHUNK_SIZE, BlockID::Air};
        glm::ivec2 m_offset{0};
    };

}

#endif //CHUNK_HPP
