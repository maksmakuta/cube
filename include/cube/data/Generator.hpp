#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <FastNoise/FastNoise.h>
#include <glm/vec3.hpp>

#include <cube/data/Chunk.hpp>

namespace cube {

    class Generator final {
    public:
        explicit Generator(int seed);
        Chunk generate(const glm::ivec3& chunkPos);

    private:
        static Block getBlockForPosition(const glm::ivec3& pos, int height, float temp);

        int m_seed;
        FastNoise::SmartNode<FastNoise::FractalFBm> m_terrainNoise;
        FastNoise::SmartNode<FastNoise::Perlin> m_tempNoise;
    };
}

#endif //CUBE_GENERATOR_HPP
