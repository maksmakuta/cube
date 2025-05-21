#include "cube/graphics/Mesher.hpp"

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

    VoxelMesh Mesher::toMesh(const ChunkPtr& chunk, const std::array<ChunkPtr,4>& neighbors,const glm::vec2& chunk_pos) {
        VoxelMesh mesh{};
        mesh.key = chunk_pos;
        uint32_t inc = 0;

        for (int x = 0; x < CHUNK_WIDTH;++x) {
            for (int z = 0; z < CHUNK_DEPTH;++z) {
                for (int y = 0; y < CHUNK_HEIGHT;++y) {
                    if(!chunk->is({x,y,z},BlockID::Air)) {
                        const auto pos = glm::ivec3{x,y,z};
                        const auto id = chunk->get(pos);

                        for (int face = 0; face < 6; face++) {
                            auto c = chunk;
                            auto n = pos + FaceOffsets[face];

                            if (n.x < 0) {
                                c = neighbors.at(1);
                                n.x += CHUNK_WIDTH;
                            }

                            if (n.x >= CHUNK_WIDTH) {
                                c = neighbors.at(0);
                                n.x -= CHUNK_WIDTH;
                            }

                            if (n.z < 0) {
                                c = neighbors.at(3);
                                n.z += CHUNK_DEPTH;
                            }

                            if (n.z >= CHUNK_DEPTH) {
                                c = neighbors.at(2);
                                n.z -= CHUNK_DEPTH;
                            }

                            if (c && c->is(n,BlockID::Air)) {
                                const auto uv = getTile(id,static_cast<Face>(face));

                                for (int i = 0; i < 4; i++) {
                                    mesh.vertices.emplace_back(
                                        glm::vec3(pos + FaceVertices[face][i]),
                                        glm::mix(glm::vec2{uv.x,uv.y},glm::vec2{uv.z,uv.w},TexCoords[i])
                                    );
                                }

                                mesh.indices.insert(mesh.indices.end(),{
                                    inc, inc + 2, inc + 1,
                                    inc, inc + 3, inc + 2,
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
