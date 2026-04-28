#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

#include <glm/vec3.hpp>

#include "cube/data/World.hpp"
#include "cube/utils/ThreadPool.hpp"

namespace cube {

    void generateChunk(const glm::ivec3& pos, World& world, ThreadPool& pool);

}

#endif //CUBE_GENERATOR_HPP
