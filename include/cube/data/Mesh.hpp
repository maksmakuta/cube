#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP

#include <memory>

#include <cube/data/Chunk.hpp>
#include <cube/graphics/RenderableMesh.hpp>

namespace cube {

    struct ChunkNeighbors {
        std::shared_ptr<Chunk> center = nullptr;
        std::shared_ptr<Chunk> px = nullptr; // +X
        std::shared_ptr<Chunk> nx = nullptr; // -X
        std::shared_ptr<Chunk> py = nullptr; // +Y
        std::shared_ptr<Chunk> ny = nullptr; // -Y
        std::shared_ptr<Chunk> pz = nullptr; // +Z
        std::shared_ptr<Chunk> nz = nullptr; // -Z

        [[nodiscard]] bool isValid() const;
        [[nodiscard]] Block getBlock(const glm::ivec3& pos) const;
    };

    RenderableMesh getMesh(
        const ChunkNeighbors& n,
        const glm::ivec3& chunkPos,
        const std::array<float,256>& temp_data,
        const std::array<float,256>& humid_data
    );

}

#endif //CUBE_MESH_HPP
