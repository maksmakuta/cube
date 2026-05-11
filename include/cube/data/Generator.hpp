#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <queue>
#include <FastNoise/FastNoise.h>
#include <glm/vec3.hpp>

#include "World.hpp"

namespace cube {

    class Generator final {
    public:
        explicit Generator(int seed = 0);

        void push(const glm::ivec3& chunk);
        void clear();
        ChunkPtr pop(glm::ivec3* pos);

        [[nodiscard]] int len() const;

    private:
        ChunkPtr generateChunk(const glm::ivec3& chunk);

        int m_seed;
        std::queue<glm::ivec3> m_chunk;
        FastNoise::SmartNode<FastNoise::Perlin> m_terrain;
    };

}

#endif //CUBE_GENERATOR_HPP
