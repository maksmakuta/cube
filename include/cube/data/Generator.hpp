#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <vector>
#include <glm/vec3.hpp>

#include <cube/data/Chunk.hpp>

namespace cube {

    class Generator final {
    public:
        explicit Generator(int seed);
        Chunk generate(const glm::ivec3& chunkPos);
        std::vector<NoiseValues> generateNoiseValues(const glm::ivec3& chunkPos);

    private:

        int m_seed;
    };
}

#endif //CUBE_GENERATOR_HPP