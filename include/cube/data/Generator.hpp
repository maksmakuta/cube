#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <glm/vec3.hpp>

#include <cube/data/Chunk.hpp>
#include <FastNoise/FastNoise.h>

namespace cube {

    class Generator final {
    public:
        explicit Generator(int seed);
        Chunk generate(const glm::ivec3& chunkPos);
        std::vector<NoiseValues> generateNoiseValues(const glm::ivec3& chunkPos);

    private:
        static Block getBlockForPosition(int y, int surfaceHeight, float temp, float moisture);

        int m_seed;
        FastNoise::SmartNode<FastNoise::FractalFBm> m_terrainNoise;
        FastNoise::SmartNode<FastNoise::Perlin> m_tempNoise;
        FastNoise::SmartNode<FastNoise::Perlin> m_moistureNoise;
    };
}

#endif //CUBE_GENERATOR_HPP
