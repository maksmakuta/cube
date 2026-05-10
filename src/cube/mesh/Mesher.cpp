#include "cube/mesh/Mesher.hpp"

#include <map>

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

    const auto FACES = std::vector<std::vector<glm::ivec3>>{
        {{1,0,1}, {1,0,0}, {1,1,0}, {1,1,1}},
        {{0,0,0}, {0,0,1}, {0,1,1}, {0,1,0}},
        {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}},
        {{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}},
        {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}},
        {{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}}
    };

    const auto TEXTURE_UV = std::vector<glm::vec2>{
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f}
    };

    inline bool outOfPlane(const int val) {
        return val < 0 || val >= CHUNK_SIZE;
    }

    inline bool isOutside(const glm::ivec3& pos) {
        return outOfPlane(pos.x) || outOfPlane(pos.y) || outOfPlane(pos.z);
    }

    struct BlockData {
        uint8_t top;
        uint8_t bottom;
        uint8_t side;
        uint8_t overlay;
        bool is_tint;
    };

    const auto BLOCKDATA = std::map<Block,BlockData>{
         { Block::Air,      {0,0,0,0, false}},
         { Block::Grass,    {0,3,3,2, true}},
         { Block::Dirt,     {3,3,3,0, false}},
         { Block::Stone,    {4,4,4,0, false}},
         { Block::Bedrock,  {5,5,5,0, false}},
    };

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

                    const auto&[top, bottom, side, overlay, is_tint] = BLOCKDATA.at(current_block);

                    for (auto dir_id = 0; dir_id < DIRECTIONS.size(); dir_id++) {
                        const auto n_block_pos = block_pos + DIRECTIONS[dir_id];

                        if (!isOutside(n_block_pos)) {
                            if (chunk->at(n_block_pos) != Block::Air) {
                                continue;
                            }
                        }

                        const auto base_index = static_cast<uint32_t>(mesh.vertices.size());

                        float tex_id = 0.0f;
                        if (dir_id == 2) {
                            tex_id = static_cast<float>(top);
                        } else if (dir_id == 3) {
                            tex_id = static_cast<float>(bottom);
                        } else {
                            tex_id = static_cast<float>(side);
                        }

                        for (auto face_vert_id = 0; face_vert_id < 4; face_vert_id++) {
                            mesh.vertices.emplace_back(
                                glm::vec3(block_pos + FACES[dir_id][face_vert_id]),
                                glm::vec3(DIRECTIONS[dir_id]),
                                glm::vec3(TEXTURE_UV[face_vert_id],tex_id),
                                 current_block == Block::Grass &&  dir_id == 2 ? glm::vec4(0.3f,0.86f,0.45f,1.f) : glm::vec4(1.f)
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