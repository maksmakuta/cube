#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include "FastNoise/FastNoise.h"
#include "models/world/World.hpp"

namespace cube {

    class Generator {
    public:
        explicit Generator(int seed = 0);

        [[nodiscard]] float getHeight(const glm::vec2& pos) const;
        [[nodiscard]] ChunkPtr generateChunk(const glm::ivec2& pos) const;

        [[nodiscard]] int getSeed() const {
            return m_seed;
        }

    private:
        int m_seed;
        FastNoise::SmartNode<> m_noise;
    };

}

#endif //CUBE_GENERATOR_HPP