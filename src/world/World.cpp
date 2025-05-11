#include "cube/world/World.hpp"

namespace cube {

    World::World() = default;
    World::~World() = default;

    Chunk World::getChunk(const glm::ivec2& pos) {
        return m_chunks.at(pos);
    }

    void World::setChunk(const glm::ivec2& pos, const Chunk& c) {
        m_chunks.insert_or_assign(pos,c);
    }

    void World::removeChunk(const glm::ivec2& pos) {
        m_chunks.erase(pos);
    }

}