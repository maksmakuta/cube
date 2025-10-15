#include "World.hpp"

namespace cube {

    World::World() = default;
    World::~World() = default;

    ChunkPtr World::getChunk(const ChunkPos& p) {
        if (m_chunks.contains(p)) {
            return m_chunks.at(p);
        }
        return nullptr;
    }

    void World::setChunk(const ChunkPos& p, ChunkPtr c) {
        m_chunks.insert_or_assign(p,c);
    }

    void World::removeChunk(const ChunkPos& p) {
        if (m_chunks.contains(p)) {
            m_chunks.erase(p);
        }
    }

}