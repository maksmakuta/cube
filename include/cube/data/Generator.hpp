#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <memory>

#include "cube/data/Chunk.hpp"
#include "FastNoise/FastNoise.h"

namespace cube {

    class Generator final {
    public:
        explicit Generator(int seed = 0);

        std::shared_ptr<Chunk> generate(const glm::ivec3& pos);
        int getHeightAt(const glm::ivec3& pos);

    private:
        int m_seed;
        FastNoise::SmartNode<> m_terrain;
    };

}

#endif //CUBE_GENERATOR_HPP
