#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <cstdint>

#include "FastNoise/FastNoise.h"
#include "models/world/World.hpp"

namespace cube {

    class Generator {
    public:
        explicit Generator(uint32_t seed = 0);

        float getHeight(const glm::vec2& pos) const;
        ChunkPtr generateChunk(const glm::ivec2& pos) const;

    private:
        uint32_t m_seed;
        FastNoise::SmartNode<> m_noise;
    };


}

#endif //CUBE_GENERATOR_HPP