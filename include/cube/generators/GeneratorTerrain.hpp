#ifndef GENERATORTERRAIN_HPP
#define GENERATORTERRAIN_HPP

#include "cube/core/IGenerator.hpp"
#include "FastNoise/FastNoise.h"

namespace cube {

    class GeneratorTerrain final : public IGenerator {
    public:
        GeneratorTerrain();
        explicit GeneratorTerrain(int seed);
        ~GeneratorTerrain() override;
        ChunkPtr generateAt(const glm::vec2 &pos) override;
        void postGenerate(const ChunkPtr &center, const std::array<ChunkPtr, 8> &neighbors,const glm::vec2 &pos) override;

    private:
        FastNoise::SmartNode<FastNoise::FractalFBm> generator;
    };

}

#endif //GENERATORTERRAIN_HPP
