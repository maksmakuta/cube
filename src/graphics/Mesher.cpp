#include "cube/graphics/Mesher.hpp"

#include <glm/common.hpp>

namespace cube {

    Mesher::Mesher() = default;
    Mesher::~Mesher() = default;

    static const glm::ivec3 FaceOffsets[6] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    static const glm::ivec3 FaceVertices[6][4] = {
        {
            {1, 1, 0},
            {1, 1, 1},
            {1, 0, 1},
            {1, 0, 0}
        },
        {
            {0, 1, 1},
            {0, 1, 0},
            {0, 0, 0},
            {0, 0, 1}
        },
        {
            {0, 1, 1},
            {1, 1, 1},
            {1, 1, 0},
            {0, 1, 0}
        },
        {
            {0, 0, 0},
            {1, 0, 0},
            {1, 0, 1},
            {0, 0, 1}
        },
        {
            {1, 1, 1},
            {0, 1, 1},
            {0, 0, 1},
            {1, 0, 1}
        },
        {
            {0, 1, 0},
            {1, 1, 0},
            {1, 0, 0},
            {0, 0, 0}
        },
    };

    static const glm::vec3 quad1[4] = {
        {0, 1, 0},
        {1, 1, 1},
        {1, 0, 1},
        {0, 0, 0}
    };

    static const glm::vec3 quad2[4] = {
        {1, 1, 0},
        {0, 1, 1},
        {0, 0, 1},
        {1, 0, 0},
    };

    const glm::vec2 TexCoords[4] = {
        {0, 0}, {1, 0}, {1, 1}, {0, 1}
    };

    VoxelMesh Mesher::toMesh(const ChunkPtr &chunk, const std::array<ChunkPtr, 8> &neighbors,const glm::vec2 &chunk_pos) {

        if (!chunk) {
            return {};
        }

        VoxelMesh mesh{};
        mesh.key = chunk_pos;
        uint32_t inc_t = 0;
        uint32_t inc_b = 0;

        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                    const auto pos = glm::ivec3{x, y, z};
                    const auto id = chunk->get(pos);

                    if (id == BlockID::Air)
                        continue;

                    if (isFullBlock(id)) {
                        for (int face = 0; face < 6; face++) {
                            auto c = chunk;
                            auto n = pos + FaceOffsets[face];

                            if (n.y < 0 || n.y > CHUNK_HEIGHT) {
                                continue;
                            }

                            if (n.x < 0) {
                                c = neighbors.at(3);
                                n.x += CHUNK_WIDTH;
                            }

                            if (n.x >= CHUNK_WIDTH) {
                                c = neighbors.at(4);
                                n.x -= CHUNK_WIDTH;
                            }

                            if (n.z < 0) {
                                c = neighbors.at(6);
                                n.z += CHUNK_DEPTH;
                            }

                            if (n.z >= CHUNK_DEPTH) {
                                c = neighbors.at(1);
                                n.z -= CHUNK_DEPTH;
                            }

                            if (c) {
                                if (!isFullBlock(c->at(n))) {
                                    const auto uv = getTile(id, static_cast<Face>(face));

                                    for (int i = 0; i < 4; i++) {
                                        mesh.terrain.vertices.emplace_back(
                                            glm::vec3(pos + FaceVertices[face][i]),
                                            glm::mix(glm::vec2{uv.x, uv.y}, glm::vec2{uv.z, uv.w}, TexCoords[i]),
                                            3
                                        );
                                    }

                                    mesh.terrain.indices.insert(mesh.terrain.indices.end(), {
                                                                    inc_t, inc_t + 2, inc_t + 1,
                                                                    inc_t, inc_t + 3, inc_t + 2,
                                                                });

                                    inc_t += 4;
                                }
                            }
                        }
                    } else {
                        const auto uv = getTile(id, Face::Front);

                        for (const auto list: {quad1, quad2}) {
                            for (int i = 0; i < 4; i++) {
                                mesh.billboard.vertices.emplace_back(
                                    glm::vec3(pos) + list[i],
                                    glm::mix(glm::vec2{uv.x, uv.y}, glm::vec2{uv.z, uv.w}, TexCoords[i]),
                                    3
                                );
                            }

                            mesh.billboard.indices.insert(
                                mesh.billboard.indices.end(),
                                {
                                    inc_b, inc_b + 2, inc_b + 1,
                                    inc_b, inc_b + 3, inc_b + 2,
                                }
                            );

                            inc_b += 4;
                        }
                    }
                }
            }
        }

        return mesh;
    }
}
