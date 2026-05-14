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
        glm::ivec2(0,1),
        glm::ivec2(1,1),
        glm::ivec2(1,0),
        glm::ivec2(0,0)
    };

    constexpr glm::ivec3 sign_mask(const glm::ivec3& v, const glm::ivec3& n) {
        glm::ivec3 r(0);

        for (int i = 0; i < 3; ++i) {
            if (n[i] == 0)
                r[i] = v[i] ? 1 : -1;
        }

        return r;
    }

    glm::ivec3 AO_NEIGHBORS[6][4][3] = {};

    constexpr auto make_ao_neighbors() {
        for (int face = 0; face < 6; ++face) {
            const auto normal = DIRECTIONS[face];

            for (int vert = 0; vert < 4; ++vert) {
                const auto s = sign_mask(FACES[face][vert], normal);

                glm::ivec3 side1(0);
                glm::ivec3 side2(0);

                int axis = 0;

                for (int i = 0; i < 3; ++i) {
                    if (normal[i] == 0) {
                        if (axis == 0)
                            side1[i] = s[i];
                        else
                            side2[i] = s[i];

                        ++axis;
                    }
                }

                AO_NEIGHBORS[face][vert][0] = side1;
                AO_NEIGHBORS[face][vert][1] = side2;
                AO_NEIGHBORS[face][vert][2] = side1 + side2;
            }
        }
    }

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

    int getAOForVertex(const glm::ivec3& blockPos, const int dir, const int vertexIdx, const std::unordered_map<glm::ivec3, ChunkPtr>& chunks) {
        const auto& sideOffset = DIRECTIONS[dir];
        const glm::ivec3 neighborPos = blockPos + sideOffset;

        const glm::ivec3 s1Pos = neighborPos + AO_NEIGHBORS[dir][vertexIdx][0];
        const glm::ivec3 s2Pos = neighborPos + AO_NEIGHBORS[dir][vertexIdx][1];
        const glm::ivec3 cPos  = neighborPos + AO_NEIGHBORS[dir][vertexIdx][2];

        const bool s1 = getBlock(s1Pos, chunks) != Block::Air;
        const bool s2 = getBlock(s2Pos, chunks) != Block::Air;
        const bool c  = getBlock(cPos, chunks)  != Block::Air;

        if (s1 && s2) return 0;
        return 3 - (static_cast<int>(s1) + static_cast<int>(s2) + static_cast<int>(c));
    }

    ChunkMesh mesh(const glm::ivec3& pos, const World& world) {
        make_ao_neighbors();
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
                    const auto block = chunks.at({0,0,0})->at(block_pos);

                    if (block == Block::Air) {
                        continue;
                    }

                    BlockInfo info = getInfo(block);

                    for (int dir = 0; dir < 6; dir++) {
                        const auto& dir_offset = DIRECTIONS[dir];
                        const auto side_pos = block_pos + dir_offset;

                        if (getBlock(side_pos, chunks) != Block::Air) {
                            continue;
                        }

                        const auto index = mesh_data.vertices.size();

                        auto tint = glm::vec3(1.0f);
                        int texId = 0;
                        int overlay = 0;

                        if (dir == 1) {
                            texId = info.texture_top;
                            if (info.is_tint) {
                                if (block == Block::Grass) {
                                    tint = glm::vec3(0.64f, 1.0f, 0.56f);
                                }else if (block == Block::Water) {
                                    tint = glm::vec3(0.12f, 0.38f, 0.73f);
                                }
                            }
                        } else if (dir == 4) {
                            texId = info.texture_bottom;
                        } else {
                            texId = info.texture_side;
                            overlay = info.texture_overlay;

                            if (info.is_tint) {
                                tint = glm::vec3(0.64f, 1.0f, 0.56f);
                            }
                        }

                        const auto itint = glm::ivec3(tint * 255.f);

                        int ao[4];
                        for(int i=0; i<4; i++)
                            ao[i] = getAOForVertex(block_pos, dir, i, chunks);

                        for (int i = 0; i < 4; i++) {
                            const auto vertex_pos = block_pos + FACES[dir][i];

                            mesh_data.vertices.emplace_back(
                                vertex_pos.x,vertex_pos.y,vertex_pos.z,
                                dir,TEX_COORDS[i].x,TEX_COORDS[i].y,
                                texId,info.anim_count,info.anim_speed,overlay,
                                itint.r,itint.g,itint.b,ao[i]
                            );
                        }

                        std::array indices = {1, 2, 3, 3, 0, 1};

                        if (ao[0] + ao[2] >= ao[1] + ao[3]) {
                            indices = {0, 1, 2, 2, 3, 0};
                        }

                        for (const auto& i : indices) {
                            mesh_data.indices.push_back(index + i);
                        }
                    }
                }
            }
        }

        return mesh_data;
    }

}