#include "cube/generators/GeneratorFlat.hpp"

#include "cube/core/Constants.hpp"

namespace cube {
    GeneratorFlat::GeneratorFlat(const int seed) : IGenerator(seed) {
    }

    int GeneratorFlat::getHeight(int x, int z) {
        return 1;
    }

    float GeneratorFlat::getProbability(int x, int z, GeneratorFeature feature) {
        return 0.f;
    }

    ChunkPtr GeneratorFlat::getChunk(const glm::ivec2 &pos) {
        const auto newChunk = std::make_shared<Chunk>();
        newChunk->fill({0, 0, 0}, {CHUNK_WIDTH, getHeight(0, 0), CHUNK_DEPTH}, BlockID::Grass);
        return newChunk;
    }
}
