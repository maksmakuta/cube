#ifndef CUBE_WORLD_HPP
#define CUBE_WORLD_HPP

#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include <unordered_set>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include "Chunk.hpp"

namespace cube {

    class World {
    public:
        using ChunkMap = std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>>;

        [[nodiscard]] uint8_t get_block(glm::ivec3 world_pos) const;
        void set_block(glm::ivec3 world_pos, uint8_t id);

        void mark_chunk_dirty(glm::ivec3 chunk_pos);
        std::vector<glm::ivec3> take_dirty_chunks();

        bool has_chunk(glm::ivec3 chunk_pos) const;
        void add_chunk(glm::ivec3 chunk_pos, std::unique_ptr<Chunk> chunk);

    private:
        ChunkMap m_chunks;
        std::unordered_set<glm::ivec3> m_dirty_chunks;
        mutable std::shared_mutex m_world_mutex;
        mutable std::mutex m_dirty_mutex;
    };

}

#endif //CUBE_WORLD_HPP
