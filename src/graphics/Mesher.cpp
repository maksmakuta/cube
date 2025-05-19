#include "cube/graphics/Mesher.hpp"

#include <queue>
#include <glm/common.hpp>

namespace cube {

    Mesher::Mesher() = default;
    Mesher::~Mesher() = default;

    const glm::ivec3 FaceOffsets[6] = {
        {  1,  0,  0 },
        { -1,  0,  0 },
        {  0,  1,  0 },
        {  0, -1,  0 },
        {  0,  0,  1 },
        {  0,  0, -1 }
    };

    const glm::ivec3 FaceVertices[6][4] = {
        { {1, 1, 0},{1, 1, 1},{1, 0, 1},{1, 0, 0} },
        { {0, 1, 1},{0, 1, 0},{0, 0, 0},{0, 0, 1} },
        { {0, 1, 1},{1, 1, 1},{1, 1, 0},{0, 1, 0} },
        { {0, 0, 0},{1, 0, 0},{1, 0, 1},{0, 0, 1} },
        { {1, 1, 1},{0, 1, 1},{0, 0, 1},{1, 0, 1} },
        { {0, 1, 0},{1, 1, 0},{1, 0, 0},{0, 0, 0} },
    };

    const glm::vec2 TexCoords[4] = {
        {0, 0}, {1, 0}, {1, 1}, {0, 1}
    };

    VoxelMesh Mesher::toMesh(World& w, const glm::ivec2& current) {
        VoxelMesh mesh{};
        mesh.key = current;

        const auto chunk = w.at(current);

        uint32_t inc = 0;

        for (int x = 0; x < CHUNK_WIDTH;++x) {
            for (int z = 0; z < CHUNK_DEPTH;++z) {
                for (int y = 0; y < CHUNK_HEIGHT;++y) {
                    if (!chunk.is({x,y,z},BlockID::Air)) {
                        const auto pos = glm::ivec3{x,y,z};
                        const auto id = chunk.get(pos);

                        for (int face = 0; face < 6; face++) {
                            // glm::ivec2 chuck_pos = current;
                            // auto temp = chunk;
                            auto n = pos + FaceOffsets[face];
/*
                            if (n.x < 0) {
                                chuck_pos += glm::ivec2{-1,0};
                                n.x += CHUNK_WIDTH;
                            }else if (n.x >= CHUNK_WIDTH) {
                                chuck_pos += glm::ivec2{1,0};
                                n.x -= CHUNK_WIDTH;
                            }

                            if (n.z < 0) {
                                chuck_pos += glm::ivec2{0,-1};
                                n.z += CHUNK_DEPTH;
                            }else if (n.z >= CHUNK_DEPTH) {
                                chuck_pos += glm::ivec2{0,1};
                                n.z -= CHUNK_DEPTH;
                            }

                            if (chuck_pos != current) {
                                temp = w.at(chuck_pos);
                            }*/

                            if (chunk.is(n,BlockID::Air)) {
                                const auto uv = getTile(id,static_cast<Face>(face));

                                for (int i = 0; i < 4; i++) {
                                    mesh.vertices.emplace_back(
                                        glm::vec3(pos + FaceVertices[face][i]),
                                        glm::mix(glm::vec2{uv.x,uv.y},glm::vec2{uv.z,uv.w},TexCoords[i])
                                    );
                                }

                                mesh.indices.insert(mesh.indices.end(),{
                                    inc, inc + 1, inc + 2,
                                    inc, inc + 2, inc + 3,
                                });

                                inc += 4;
                            }
                        }
                    }
                }
            }
        }

        return mesh;
    }

}
