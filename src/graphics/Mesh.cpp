#include "cube/graphics/Mesh.hpp"

namespace cube {
    Mesh::Mesh() = default;
    Mesh::~Mesh() = default;

    constexpr glm::ivec3 faceVertexOffsets[6][4] = {
        { {1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1} },
        { {0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {0, 0, 0} },
        { {0, 1, 1}, {1, 1, 1}, {1, 1, 0}, {0, 1, 0} },
        { {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1} },
        { {1, 0, 1}, {1, 1, 1}, {0, 1, 1}, {0, 0, 1} },
        { {0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0} }
    };

    constexpr glm::ivec3 faceNormals[6] = {
        { 1,  0,  0},
        {-1,  0,  0},
        { 0,  1,  0},
        { 0, -1,  0},
        { 0,  0,  1},
        { 0,  0, -1}
    };

    constexpr glm::vec2 faceUVs[4] = {
        {0, 0}, {0, 1}, {1, 1}, {1, 0}
    };

    ChunkMesh Mesh::toMesh(Chunk& c) {
        ChunkMesh mesh{};

        for (int x = 0; x < CHUNK_WIDTH; ++x){
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                for (int z = 0; z < CHUNK_DEPTH; ++z) {
                    const auto block = c.at({x, y, z});
                    if (block == BlockID::Air)
                        continue;

                    for (int face = 0; face < 6; ++face) {
                        if (glm::ivec3 neighbor = glm::ivec3(x, y, z) + faceNormals[face]; c.is(neighbor,BlockID::Air)) {
                            for (const auto i : std::vector{0,1,2,0,2,3}) {
                                Vertex3D v{};
                                v.pos = glm::ivec3(x, y, z) + faceVertexOffsets[face][i];
                                v.tex = faceUVs[i];
                                mesh.push_back(v);
                            }
                        }
                    }
                }
            }
        }
        return mesh;
    }
}