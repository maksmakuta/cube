#include "cube/graphics/Mesher.hpp"

#include <iostream>
#include <glm/common.hpp>

#include "cube/core/Constants.hpp"

namespace cube {

    constexpr glm::ivec3 normals[6] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    constexpr glm::ivec3 vertices[6][4] = {
        {
            {1, 0, 0},
            {1, 0, 1},
            {1, 1, 1},
            {1, 1, 0}
        },
        {
            {0, 0, 0},
            {0, 0, 1},
            {0, 1, 1},
            {0, 1, 0}
        },
        {
            {0, 1, 0},
            {1, 1, 0},
            {1, 1, 1},
            {0, 1, 1},
        },
        {
            {1, 0, 1},
            {1, 0, 0},
            {0, 0, 0},
            {0, 0, 1},
        },
        {
            {1, 0, 1},
            {0, 0, 1},
            {0, 1, 1},
            {1, 1, 1},
        },
        {
            {1, 0, 0},
            {0, 0, 0},
            {0, 1, 0},
            {1, 1, 0},
        }
    };

    constexpr glm::vec2 uv_coords[4] = {
        {0, 1},
        {1, 1},
        {1, 0},
        {0, 0},
    };

    bool isFullBlock(const BlockID i) {
        if (i == BlockID::MushroomBrown ||
            i == BlockID::MushroomRed ||
            i == BlockID::Weed ||
            i == BlockID::WeedDry)
            return false;
        return true;
    }

    int computeAO(const bool side1, const bool side2, const bool corner) {
        return side1 && side2 ? 0 : (side1 + side2 + corner);
    }

    Mesh toMesh(const ChunkPtr &chunk, const std::array<ChunkPtr, 4> &neighbours) {
        if (!chunk) return {};

        Mesh mesh{};
        uint32_t index = 0;
        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                    const auto pos = glm::ivec3{x, y, z};
                    if (chunk->getBlock(pos) == BlockID::Air || y == 0) {
                        continue;
                    }

                    for (int f = 0; f < 6; f++) {
                        ChunkPtr c = chunk;
                        auto n = pos + normals[f];

                        if (n.x >= CHUNK_WIDTH) {
                            c = neighbours[0];
                            n.x -= CHUNK_WIDTH;
                        } else if (n.x < 0) {
                            c = neighbours[1];
                            n.x += CHUNK_WIDTH;
                        }

                        if (n.z >= CHUNK_DEPTH) {
                            c = neighbours[2];
                            n.z -= CHUNK_DEPTH;
                        } else if (n.z < 0) {
                            c = neighbours[3];
                            n.z += CHUNK_DEPTH;
                        }

                        if (c && c->getBlock(n) == BlockID::Air) {

                            const auto face = static_cast<Face>(f);
                            const auto uv = getTile(chunk->getBlock(pos), face);
                            for (int i = 0; i < 4; ++i) {
                                int ao = 0;
                                for (const auto& r  : {0,1,4,5}) {
                                    ChunkPtr c2 = chunk;
                                    auto n2 = pos + normals[r];

                                    if (n2.x >= CHUNK_WIDTH) {
                                        c2 = neighbours[0];
                                        n2.x -= CHUNK_WIDTH;
                                    } else if (n2.x < 0) {
                                        c2 = neighbours[1];
                                        n2.x += CHUNK_WIDTH;
                                    }

                                    if (n2.z >= CHUNK_DEPTH) {
                                        c2 = neighbours[2];
                                        n2.z -= CHUNK_DEPTH;
                                    } else if (n2.z < 0) {
                                        c2 = neighbours[3];
                                        n2.z += CHUNK_DEPTH;
                                    }

                                    if (c2) {
                                        ao += c2->getBlock(n2) != BlockID::Air;
                                    }
                                }

                                mesh.vertices.emplace_back(
                                    pos + vertices[f][i],
                                    glm::mix(glm::vec2{uv.x, uv.y}, glm::vec2{uv.z, uv.w}, uv_coords[i]),
                                    ao
                                );
                            }


                            const auto indices = (f & 1)
                                ? std::initializer_list{
                                    index, index + 1, index + 2, index, index + 2, index + 3
                                }
                                : std::initializer_list{
                                    index, index + 2, index + 1, index, index + 3, index + 2
                                };

                            mesh.indices.insert(
                                mesh.indices.end(),
                                indices
                            );

                            index += 4;
                        }
                    }
                }
            }
        }
        return mesh;
    }

}
