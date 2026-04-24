#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP

#include <cube/data/Chunk.hpp>
#include <glm/vec2.hpp>

#include "cube/graphics/RenderableMesh.hpp"

namespace cube {

    struct ChunkNeighbors {
        const Chunk* center = nullptr;
        const Chunk* px = nullptr; // +X
        const Chunk* nx = nullptr; // -X
        const Chunk* py = nullptr; // +Y
        const Chunk* ny = nullptr; // -Y
        const Chunk* pz = nullptr; // +Z
        const Chunk* nz = nullptr; // -Z

        bool isValid() const;
        Block getBlock(const glm::ivec3& pos) const;
    };

    RenderableMesh getMesh(const ChunkNeighbors& n, const glm::ivec3& chunkPos);

}

#endif //CUBE_MESH_HPP
