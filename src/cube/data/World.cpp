#include <cube/data/World.hpp>
#include <mutex>

namespace cube {

    static constexpr int SHIFT = 4;

    uint8_t World::get_block(const glm::ivec3 world_pos) const {
        const glm::ivec3 chunk_pos = {
            world_pos.x >> SHIFT,
            world_pos.y >> SHIFT,
            world_pos.z >> SHIFT
        };

        const glm::ivec3 local_pos = {
            world_pos.x & CHUNK_SIZE.x - 1,
            world_pos.y & CHUNK_SIZE.y - 1,
            world_pos.z & CHUNK_SIZE.z - 1
        };

        std::shared_lock lock(m_world_mutex);
        if (const auto it = m_chunks.find(chunk_pos); it != m_chunks.end()) {
            return it->second->view()[local_pos.x, local_pos.y, local_pos.z];
        }
        return 0;
    }

    void World::set_block(glm::ivec3 world_pos, uint8_t id) {
        const glm::ivec3 chunk_pos = {
            world_pos.x >> SHIFT,
            world_pos.y >> SHIFT,
            world_pos.z >> SHIFT
        };

        const glm::ivec3 local_pos = {
            world_pos.x & CHUNK_SIZE.x - 1,
            world_pos.y & CHUNK_SIZE.y - 1,
            world_pos.z & CHUNK_SIZE.z - 1
        };

        {
            std::unique_lock lock(m_world_mutex);

            auto it = m_chunks.find(chunk_pos);
            if (it == m_chunks.end()) {
                if (id == 0) return;

                m_chunks[chunk_pos] = std::make_unique<Chunk>();
                it = m_chunks.find(chunk_pos);
            }

            it->second->view()[local_pos.x, local_pos.y, local_pos.z] = id;
        }

        mark_chunk_dirty(chunk_pos);

        if (local_pos.x == 0)                   mark_chunk_dirty(chunk_pos - glm::ivec3(1, 0, 0));
        if (local_pos.x == CHUNK_SIZE.x - 1)    mark_chunk_dirty(chunk_pos + glm::ivec3(1, 0, 0));

        if (local_pos.y == 0)                   mark_chunk_dirty(chunk_pos - glm::ivec3(0, 1, 0));
        if (local_pos.y == CHUNK_SIZE.x - 1)    mark_chunk_dirty(chunk_pos + glm::ivec3(0, 1, 0));

        if (local_pos.z == 0)                   mark_chunk_dirty(chunk_pos - glm::ivec3(0, 0, 1));
        if (local_pos.z == CHUNK_SIZE.x - 1)    mark_chunk_dirty(chunk_pos + glm::ivec3(0, 0, 1));
    }

    void World::mark_chunk_dirty(const glm::ivec3 chunk_pos) {
        std::lock_guard lock(m_dirty_mutex);
        m_dirty_chunks.insert(chunk_pos);
    }

    std::vector<glm::ivec3> World::take_dirty_chunks() {
        std::lock_guard lock(m_dirty_mutex);
        if (m_dirty_chunks.empty()) return {};
        std::vector to_process(m_dirty_chunks.begin(), m_dirty_chunks.end());
        m_dirty_chunks.clear();
        return to_process;
    }

    bool World::has_chunk(const glm::ivec3 chunk_pos) const {
        std::shared_lock lock(m_world_mutex);
        return m_chunks.contains(chunk_pos);
    }

    void World::add_chunk(const glm::ivec3 chunk_pos, std::unique_ptr<Chunk> chunk) {
        std::unique_lock lock(m_world_mutex);
        m_chunks[chunk_pos] = std::move(chunk);
        mark_chunk_dirty(chunk_pos);
    }

}
