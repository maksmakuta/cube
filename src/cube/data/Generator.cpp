#include "cube/data/Generator.hpp"

namespace cube {

    Generator::Generator(const int seed) : m_seed(seed) {

    }

    ChunkPtr Generator::generateChunk(const glm::ivec3& pos) {
        if (pos != glm::ivec3()) {
            return {};
        }
        const auto chunk = std::make_shared<Chunk>();

        int val = 0;

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    chunk->at({x, y, z}) = static_cast<Block>(val);
                }
            }
        }

        return chunk;
    }

    int Generator::heightAt(const glm::vec3& pos) const {
        return 0;
    }

    void Generator::setSeed(const int seed) {
        m_seed = seed;
    }

    int Generator::getSeed() const {
        return m_seed;
    }

}