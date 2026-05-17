#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <memory>

#include "cube/data/Chunk.hpp"

namespace cube {

    class Generator final {
    public:
        explicit Generator(int seed = 0);

        std::shared_ptr<Chunk> generate(const glm::ivec3& pos);
        int getHeightAt(const glm::ivec3& pos);

    private:
        int m_seed;
    };

}

#endif //CUBE_GENERATOR_HPP
