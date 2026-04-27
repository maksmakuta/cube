#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <array>
#include <glm/vec3.hpp>

#include <FastNoise/FastNoise.h>
#include <cube/data/Chunk.hpp>

namespace cube {

    constexpr auto WATER_LEVEL = 160;

    class Generator final {
    public:
        explicit Generator(int seed);

        [[nodiscard]] Chunk generate(const glm::ivec3& chunkPos) const;

        [[nodiscard]] std::array<float, 256> getTemperature(const glm::ivec3& chunkPos) const;
        [[nodiscard]] std::array<float, 256> getHumidity(const glm::ivec3& chunkPos) const;
        [[nodiscard]] std::array<float, 256> getTerrain(const glm::ivec3& chunkPos) const;
        [[nodiscard]] std::array<float, 256> getTrees(const glm::ivec3& chunkPos) const;
        [[nodiscard]] std::array<float, 256> getDecorations(const glm::ivec3& chunkPos) const;

    private:
        int m_seed;

        FastNoise::SmartNode<> m_terrain;
        FastNoise::SmartNode<> m_temperature;
        FastNoise::SmartNode<> m_humidity;
        FastNoise::SmartNode<> m_tree;
        FastNoise::SmartNode<> m_decorations;
    };
}

#endif //CUBE_GENERATOR_HPP