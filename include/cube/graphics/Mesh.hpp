#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP
#include <glm/vec3.hpp>

#include "Renderer.hpp"
#include "cube/data/Chunk.hpp"
#include "cube/data/World.hpp"
#include "cube/utils/ThreadPool.hpp"

namespace cube {

    void mesh(const glm::ivec3& pos, const World& world, Renderer& renderer, ThreadPool& threadPool);

}

#endif //CUBE_MESH_HPP
