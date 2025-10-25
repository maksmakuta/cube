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

    constexpr glm::vec3 DIRECTIONS[6] = {
        { 1, 0, 0},
         {-1, 0, 0},
         { 0, 1, 0},
         { 0,-1, 0},
         { 0, 0, 1},
         { 0, 0,-1},
    };

    constexpr glm::vec2 TEXTURE_UV[4] = {
        {0,0},
        {0,1},
        {1,1},
        {1,0},
    };

    constexpr int INDICES[6] = {
        0, 1, 2,
        2, 3, 0
    };

    const glm::vec3 FACES[6][4] = {
        {
            {1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
        },
        {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 1.0f},
        },
        {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f},
        },
        {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 1.0f},
        },
        {
            {0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
        },
        {
            {1.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
        },
    };

    ChunkMesh toMesh(const ChunkPtr& current, const std::vector<ChunkPtr>& neighbours) {
        auto mesh = ChunkMesh();
        if (current) {
            for (auto x = 0; x < CHUNK_SIZE.x; x++) {
                for (auto z = 0; z < CHUNK_SIZE.z; z++) {
                    for (auto y = 0; y < CHUNK_SIZE.y; y++) {
                        const auto pos = glm::vec3{x,y,z};
                        const auto block = current->getBlock(pos);
                        if (block == Block::Air) continue;
                        const auto [top, side, bottom] = getMetadata(block);
                        for (auto i = 0; i < 6; i++) {
                            auto chunk = current;
                            auto pos2 = pos + DIRECTIONS[i];
                            if (pos2.x < 0) {
                                pos2.x += CHUNK_SIZE.x;
                                chunk = neighbours[1];
                            }else if (pos2.x >= CHUNK_SIZE.x){
                                pos2.x -= CHUNK_SIZE.x;
                                chunk = neighbours[0];
                            }

                            if (pos2.z < 0) {
                                pos2.z += CHUNK_SIZE.z;
                                chunk = neighbours[3];
                            }else if (pos2.z >= CHUNK_SIZE.z){
                                pos2.z -= CHUNK_SIZE.z;
                                chunk = neighbours[2];
                            }
                            if (!chunk) continue;
                            if (chunk->getBlock(pos2) == Block::Air) {
                                auto tex = i == 2 ? top : i == 3 ? bottom : side;
                                for (int j = 0; j < 4; j++) {
                                    mesh.vertices.emplace_back(pos + FACES[i][j], DIRECTIONS[i], TEXTURE_UV[j], tex);
                                }
                                const auto last = mesh.vertices.size();
                                for (const auto& indice : INDICES) {
                                    mesh.indices.emplace_back(last + indice);
                                }
                            }
                        }
                    }
                }
            }
        }
        return mesh;
    }


}
