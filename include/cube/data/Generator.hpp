#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <FastNoise/FastNoise.h>
#include <glm/vec3.hpp>

#include "World.hpp"

namespace cube {

    constexpr auto WATER_LEVEL = 64;
    constexpr auto MOUNTAIN_LEVEL = 196;
    constexpr auto WORLD_LIMIT = 256;

    class Generator final {
    public:
        explicit Generator(int seed = 0);

        ChunkPtr generateChunk(const glm::ivec3& pos);
        [[nodiscard]] int heightAt(const glm::vec3& pos) const;

        void setSeed(int seed);
        [[nodiscard]] int getSeed() const;

    private:

        int m_seed;
        FastNoise::SmartNode<> m_terrain_data;
        FastNoise::SmartNode<> m_terrain;
        FastNoise::SmartNode<> m_temperature;
        FastNoise::SmartNode<> m_humidity;
        FastNoise::SmartNode<> m_caveNoise;
    };

}

#endif //CUBE_GENERATOR_HPP
