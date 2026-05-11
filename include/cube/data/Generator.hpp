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

    private:

        int m_seed;
        FastNoise::SmartNode<FastNoise::Perlin> m_terrain;
    };

}

#endif //CUBE_GENERATOR_HPP
