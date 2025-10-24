#include "ChunkMesh.hpp"

namespace cube {

    void ChunkMesh::reserve(const size_t v, const size_t i) {
        vertices.reserve(v);
        indices.reserve(i);
    }

    void ChunkMesh::clear() {
        vertices.clear();
        indices.clear();
    }

    ChunkMesh toMesh(const ChunkPtr& current, const std::vector<ChunkPtr>& neighbours) {
        //TODO (greedy meshing algorithm here)
        return {};
    }


}
