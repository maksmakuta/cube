#include "cube/graphics/Mesh.hpp"

namespace cube {
    Mesh::Mesh() = default;
    Mesh::~Mesh() = default;

    ChunkMesh Mesh::toMesh(Chunk& c) {
        return {};
    }
}
