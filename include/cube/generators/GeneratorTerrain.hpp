#ifndef GENERATORTERRAIN_HPP
#define GENERATORTERRAIN_HPP

#include "cube/core/IGenerator.hpp"
#include "FastNoise/FastNoise.h"

namespace cube {

    class GeneratorTerrain final : public IGenerator {
    public:
        explicit GeneratorTerrain(int seed = 0);
        ~GeneratorTerrain() override;
        Chunk generateAt(const glm::vec2 &pos) override;
    private:
        FastNoise::SmartNode<FastNoise::FractalFBm> generator;
    };

}

#endif //GENERATORTERRAIN_HPP
