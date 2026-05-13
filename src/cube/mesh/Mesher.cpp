#include "cube/mesh/Mesher.hpp"
#include "cube/data/World.hpp"

namespace cube {

    constexpr glm::ivec3 DIRECTIONS[6] = {
        glm::ivec3( 1, 0, 0),
        glm::ivec3( 0, 1, 0),
        glm::ivec3( 0, 0, 1),
        glm::ivec3(-1, 0, 0),
        glm::ivec3( 0,-1, 0),
        glm::ivec3( 0, 0,-1)
    };

    constexpr glm::ivec3 FACES[6][4] = {
        { {1, 0, 1}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1} },
        { {0, 1, 1}, {1, 1, 1}, {1, 1, 0}, {0, 1, 0} },
        { {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} },
        { {0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0} },
        { {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1} },
        { {1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0} }
    };

    constexpr glm::ivec2 TEX_COORDS[4] = {
        glm::ivec2(0,0),
        glm::ivec2(1,0),
        glm::ivec2(1,1),
        glm::ivec2(0,1)
    };

    bool isReadyForMesh(const glm::ivec3& pos, const World& world) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    if (!world.contains(pos + glm::ivec3{dx, dy, dz})) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    Block getBlock(const glm::ivec3& p, const std::unordered_map<glm::ivec3, ChunkPtr>& chunks) {
        auto index = glm::ivec3(0);
        auto pos = p;

        if (pos.x < 0           ){ index.x -= 1; pos.x += CHUNK_SIZE; }
        if (pos.x >= CHUNK_SIZE ){ index.x += 1; pos.x -= CHUNK_SIZE; }
        if (pos.y < 0           ){ index.y -= 1; pos.y += CHUNK_SIZE; }
        if (pos.y >= CHUNK_SIZE ){ index.y += 1; pos.y -= CHUNK_SIZE; }
        if (pos.z < 0           ){ index.z -= 1; pos.z += CHUNK_SIZE; }
        if (pos.z >= CHUNK_SIZE ){ index.z += 1; pos.z -= CHUNK_SIZE; }

        return chunks.at(index) ? chunks.at(index)->at(pos) : Block::Air;
    }

    ChunkMesh mesh(const glm::ivec3& pos, const World& world) {
        std::unordered_map<glm::ivec3, ChunkPtr> chunks;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    const auto delta = glm::ivec3(dx,dy,dz);
                    chunks[delta] = world.getChunk(pos + delta);
                }
            }
        }

        if (!chunks[{0,0,0}]) {
            return {};
        }

        ChunkMesh mesh_data{};
        mesh_data.vertices.reserve(512);
        mesh_data.indices.reserve(1024);

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    const auto block_pos = glm::ivec3(x,y,z);

                    if (chunks.at({0,0,0})->at(block_pos) == Block::Air) {
                        continue;
                    }

                    for (int dir = 0; dir < 6; dir++) {
                        const auto& dir_offset = DIRECTIONS[dir];
                        const auto side_pos = block_pos + dir_offset;

                        if (getBlock(side_pos, chunks) != Block::Air) {
                            continue;
                        }

                        const auto index = mesh_data.vertices.size();

                        for (int i = 0; i < 4; i++) {
                            const auto vertex_pos = block_pos + FACES[dir][i];

                            mesh_data.vertices.emplace_back(
                                vertex_pos.x,vertex_pos.y,vertex_pos.z,
                                dir,TEX_COORDS[i].x,TEX_COORDS[i].y,
                                3,1,0,0,
                                255,255,255,3
                            );
                        }

                        for (const auto& i : {0,1,2,2,3,0}) {
                            mesh_data.indices.push_back(index + i);
                        }

                    }
                }
            }
        }

        return mesh_data;
    }

}