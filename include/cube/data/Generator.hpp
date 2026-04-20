#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <glm/vec3.hpp>

#include "Chunk.hpp"
#include <FastNoise/FastNoise.h>

namespace cube {

    class Generator final {
    public:
        explicit Generator(int seed);

        void fill_chunk(glm::ivec3 chunk_pos, Chunk& chunk);

    private:
        int m_seed;
        FastNoise::SmartNode<FastNoise::FractalFBm> m_noise;
    };

}

#endif //CUBE_GENERATOR_HPP
