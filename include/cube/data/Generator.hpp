#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <FastNoise/FastNoise.h>
#include <glm/vec3.hpp>

#include "World.hpp"

namespace cube {

    class Generator final {
    public:
        explicit Generator(int seed = 0);

        ChunkPtr generateChunk(const glm::ivec3& pos);
        void setSeed(int seed);

        [[nodiscard]] int getSeed() const;
        [[nodiscard]] int heightAt(const glm::vec3& pos) const;

    private:

        int m_seed {};

        FastNoise::SmartNode<> m_terrain;
        FastNoise::SmartNode<> m_caves;
    };

}

#endif