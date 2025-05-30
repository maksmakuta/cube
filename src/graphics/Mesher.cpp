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

    constexpr glm::ivec3 aoDirections[6][4][3] = {
        // +X face
        {
            {{0, -1,  0}, {0,  0, -1}, {0, -1, -1}}, // Bottom-Back
            {{0, -1,  0}, {0,  0,  1}, {0, -1,  1}}, // Bottom-Front
            {{0,  1,  0}, {0,  0,  1}, {0,  1,  1}}, // Top-Front
            {{0,  1,  0}, {0,  0, -1}, {0,  1, -1}}, // Top-Back
        },
        // -X face
        {
            {{0, -1,  0}, {0,  0,  1}, {0, -1,  1}},
            {{0, -1,  0}, {0,  0, -1}, {0, -1, -1}},
            {{0,  1,  0}, {0,  0, -1}, {0,  1, -1}},
            {{0,  1,  0}, {0,  0,  1}, {0,  1,  1}},
        },
        // +Y face
        {
            {{1,   1,  0}, {0,  1, -1}, {1,   1, -1}},
            {{1,   1,  0}, {0,  1,  1}, {1,   1,  1}},
            {{-1,  1,  0}, {0,  1,  1}, {-1,  1,  1}},
            {{-1,  1,  0}, {0,  1, -1}, {-1,  1, -1}},
        },
        // -Y face
        {
            {{1,  0,  0}, {0,  0, -1}, {1,  0, -1}},
            {{-1, 0,  0}, {0,  0, -1}, {-1, 0, -1}},
            {{-1, 0,  0}, {0,  0,  1}, {-1, 0,  1}},
            {{1,  0,  0}, {0,  0,  1}, {1,  0,  1}},
        },
        // +Z face
        {
            {{-1,  0,  0}, {0, -1,  0}, {-1, -1,  0}},
            {{1,   0,  0}, {0, -1,  0}, {1,  -1,  0}},
            {{1,   0,  0}, {0,  1,  0}, {1,   1,  0}},
            {{-1,  0,  0}, {0, 1, 0}, {-1, 1, 0}},
        },
        // -Z face
        {
            {{1, 0, 0}, {0, -1, 0}, {1, -1, 0}},
            {{-1, 0, 0}, {0, -1, 0}, {-1, -1, 0}},
            {{-1, 0, 0}, {0, 1, 0}, {-1, 1, 0}},
            {{1, 0, 0}, {0, 1, 0}, {1, 1, 0}},
        }
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

    bool isSolid(glm::ivec3 pos, const ChunkPtr & chunk, const std::array<ChunkPtr, 4> & neighbours) {
        ChunkPtr c = chunk;

        if (pos.x >= CHUNK_WIDTH) {
            c = neighbours[0];
            pos.x -= CHUNK_WIDTH;
        } else if (pos.x < 0) {
            c = neighbours[1];
            pos.x += CHUNK_WIDTH;
        }

        if (pos.z >= CHUNK_DEPTH) {
            c = neighbours[2];
            pos.z -= CHUNK_DEPTH;
        } else if (pos.z < 0) {
            c = neighbours[3];
            pos.z += CHUNK_DEPTH;
        }

        if (c) {
            return c->getBlock(pos) != BlockID::Air;
        }
        return false;
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

                    const auto f = 2;
//                    for (int f = 0; f < 6; f++) {
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

                            auto temp = std::vector<Vertex3D>();

                            for (int i = 0; i < 4; ++i) {
                                temp.emplace_back(
                                    pos + vertices[f][i],
                                    glm::mix(glm::vec2{uv.x, uv.y}, glm::vec2{uv.z, uv.w}, uv_coords[i]),
                                    3 - computeAO(
                                        isSolid(pos + aoDirections[f][i][0], chunk, neighbours),
                                        isSolid(pos + aoDirections[f][i][1], chunk, neighbours),
                                        isSolid(pos + aoDirections[f][i][2], chunk, neighbours)
                                    )
                                );
                            }

                            if (
                                temp[0].ao + temp[3].ao < temp[1].ao + temp[2].ao
                            ) {
                                std::swap(temp[0].ao,temp[2].ao);
                                std::swap(temp[1].ao,temp[3].ao);
                            }

                            mesh.vertices.insert(mesh.vertices.end(), temp.begin(), temp.end());
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
//                }
            }
        }
        return mesh;
    }

}
