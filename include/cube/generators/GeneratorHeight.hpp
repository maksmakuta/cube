#ifndef GENERATORHEIGTH_HPP
#define GENERATORHEIGTH_HPP

#include "cube/core/IGenerator.hpp"
#include "FastNoise/FastNoise.h"

namespace cube {

    class GeneratorHeight final : public IGenerator {
    public:
        explicit GeneratorHeight(int seed = 0);
        ~GeneratorHeight() override;
        ChunkPtr getChunk(const glm::ivec2 &pos) override;
        int getHeight(int x, int z) override;
        float getProbability(int x, int z, GeneratorFeature feature) override;
    private:
        FastNoise::SmartNode<> m_noise;
    };

}

#endif //GENERATORHEIGTH_HPP
