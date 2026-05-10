#include "cube/mesh/Mesher.hpp"
#include "cube/utils/Log.hpp"

namespace cube {

    const auto DIRECTIONS = std::vector<glm::ivec3>{
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    const auto TEXTURE_UV = std::vector<glm::vec2>{
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f}
    };

    const auto FACES = std::vector<std::vector<glm::ivec3>>{
        {{1,0,1}, {1,1,1}, {1,1,0}, {1,0,0}},
        {{0,0,0}, {0,1,0}, {0,1,1}, {0,0,1}},
        {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}},
        {{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}},
        {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}},
        {{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}}
    };

    inline bool outOfPlane(const int val) {
        return val < 0 || val >= CHUNK_SIZE;
    }

    inline bool isOutside(const glm::ivec3& pos) {
        return outOfPlane(pos.x) || outOfPlane(pos.y) || outOfPlane(pos.z);
    }

    ChunkMesh mesh(const glm::ivec3& chunk_pos, const World& world) {
        const auto chunk = world.getChunk(chunk_pos);
        if (!chunk) {
            warn("Cannot load chunk [{},{},{}]", chunk_pos.x, chunk_pos.y, chunk_pos.z);
            return {};
        }

        ChunkMesh mesh{};

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    const auto block_pos = glm::ivec3(x, y, z);
                    const auto current_block = chunk->at(block_pos);

                    if (current_block == Block::Air) continue;

                    for (auto dir_id = 0; dir_id < DIRECTIONS.size(); dir_id++) {
                        const auto n_block_pos = block_pos + DIRECTIONS[dir_id];

                        if (!isOutside(n_block_pos)) {
                            if (chunk->at(n_block_pos) != Block::Air) {
                                continue;
                            }
                        }

                        const auto base_index = static_cast<uint32_t>(mesh.vertices.size());

                        for (auto face_vert_id = 0; face_vert_id < 4; face_vert_id++) {
                            mesh.vertices.emplace_back(
                                glm::vec3(block_pos + FACES[dir_id][face_vert_id]),
                                glm::vec3(DIRECTIONS[dir_id]),
                                TEXTURE_UV[face_vert_id],
                                static_cast<float>(current_block)
                            );
                        }

                        for (const auto& offset : {0, 1, 2, 2, 3, 0}) {
                            mesh.indices.push_back(base_index + offset);
                        }
                    }
                }
            }
        }

        return mesh;
    }
}