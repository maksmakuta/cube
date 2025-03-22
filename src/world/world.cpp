#include "cube/world/world.hpp"

#include <memory>
#include <optional>

#include "cube/base/generator.hpp"
#include "cube/generators/generator_flat.hpp"

namespace cube {

    namespace detail {

        Chunk generate(const int x, const int z, const std::shared_ptr<IGenerator> &generator) {
            Chunk chunk;
            for (int _x = 0; _x < CHUNK_WIDTH; ++_x) {
                for (int _y = 0; _y < CHUNK_HEIGHT; ++_y) {
                    for (int _z = 0; _z < CHUNK_DEPTH; ++_z) {
                        chunk.set(_x,_y,_z, generator->at(_x + CHUNK_WIDTH * x, _y ,_z + CHUNK_DEPTH * z));
                    }
                }
            }
            return chunk;
        }

    }

    World::World(const Seed& seed) : generator(new generators::GeneratorFlat()) {
        generator->seed(seed);
    }

    void World::loadChunk(const int x, const int z) {
        if (!m_chunks.contains(glm::ivec2(x, z))) {
            m_chunks[glm::ivec2(x, z)] = detail::generate(x, z, generator);
        }
    }

    void World::unloadChunk(const int x, const int z) {
        if (m_chunks.contains(glm::ivec2(x, z))) {
            m_chunks.erase(glm::ivec2(x, z));
        }
    }

    std::optional<Chunk> World::getChunk(const int x, const int z) {
        if (m_chunks.contains(glm::ivec2(x, z))) {
            return m_chunks[glm::ivec2(x, z)];
        }
        return std::nullopt;
    }

    void World::setChunk(const int x, const int z, const Chunk& chunk) {
        m_chunks[glm::ivec2(x, z)] = chunk;
    }

}
