#ifndef GENERATORFLAT_HPP
#define GENERATORFLAT_HPP

#include "cube/core/IGenerator.hpp"

namespace cube {
    class GeneratorFlat final : public IGenerator {
    public:
        explicit GeneratorFlat(int seed = 0);

        int getHeight(int x, int z) override;

        float getProbability(int x, int z, GeneratorFeature feature) override;

        ChunkPtr getChunk(const glm::ivec2 &pos) override;
    };
}

#endif //GENERATORFLAT_HPP
