#include "cube/mesh/Mesher.hpp"

#include <map>

#include "cube/utils/Log.hpp"

namespace cube {

    struct BlockData {
        int8_t top;
        int8_t bottom;
        int8_t side;
        int8_t overlay;
        bool is_tint;
    };

    const auto BLOCKDATA = std::map<Block,BlockData>{
         { Block::Air,      {-1,-1,-1,-1, false }},
         { Block::Grass,    { 0, 3, 1, 2, true  }},
         { Block::Dirt,     { 3, 3, 3,-1, false }},
         { Block::Stone,    { 4, 4, 4,-1, false }},
         { Block::Bedrock,  { 5, 5, 5,-1, false }},
    };

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

    struct ChunkData {
        ChunkPtr center;

        ChunkPtr top;
        ChunkPtr bottom;
        ChunkPtr near;
        ChunkPtr far;
        ChunkPtr left;
        ChunkPtr right;
    };

    ChunkMesh mesh(const glm::ivec3& pos, const World& world) {
        ChunkData data{};

        data.center = world.getChunk(pos);
        data.top    = world.getChunk(pos + glm::ivec3{0,1,0});
        data.bottom = world.getChunk(pos + glm::ivec3{0,-1,0});
        data.near   = world.getChunk(pos + glm::ivec3{0,0,1});
        data.far    = world.getChunk(pos + glm::ivec3{0,0,-1});
        data.left   = world.getChunk(pos + glm::ivec3{1,0,0});
        data.right  = world.getChunk(pos + glm::ivec3{-1,0,0});

        ChunkMesh mesh{};

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    const auto block_pos = glm::ivec3(x, y, z);
                    const auto current_block = data.center->at(block_pos);

                    if (current_block == Block::Air) continue;

                    const auto&[top, bottom, side, overlay, is_tint] = BLOCKDATA.at(current_block);

                    for (auto dir_id = 0; dir_id < DIRECTIONS.size(); dir_id++) {
                        const auto n_block_pos = block_pos + DIRECTIONS[dir_id];

                        auto chunk = data.center;
                        auto pos_normal = n_block_pos;

                        if (n_block_pos.y >= CHUNK_SIZE){ chunk = data.top;      pos_normal.y -= CHUNK_SIZE; }
                        if (n_block_pos.y < 0          ){ chunk = data.bottom;   pos_normal.y += CHUNK_SIZE; }
                        if (n_block_pos.x >= CHUNK_SIZE){ chunk = data.left;     pos_normal.x -= CHUNK_SIZE; }
                        if (n_block_pos.x < 0          ){ chunk = data.right;    pos_normal.x += CHUNK_SIZE; }
                        if (n_block_pos.z >= CHUNK_SIZE){ chunk = data.near;     pos_normal.z -= CHUNK_SIZE; }
                        if (n_block_pos.z < 0          ){ chunk = data.far;      pos_normal.z += CHUNK_SIZE; }

                        if (chunk->at(pos_normal) != Block::Air) continue;

                        const auto base_index = static_cast<uint32_t>(mesh.vertices.size());

                        float tex_id = 0.0f;
                        float current_overlay = -1.0f;
                        auto tint = glm::vec4(1.0f);

                        if (dir_id == 2) {
                            tex_id = static_cast<float>(top);
                            if (is_tint) tint = glm::vec4(0.10f, 0.70f, 0.15f, 1.0f);
                        }
                        else if (dir_id == 3) {
                            tex_id = static_cast<float>(bottom);
                        }
                        else {
                            tex_id = static_cast<float>(side);
                            if (overlay >= 0) {
                                current_overlay = static_cast<float>(overlay);
                                if (is_tint) tint = glm::vec4(0.10f, 0.70f, 0.15f, 1.0f);
                            }
                        }

                        for (auto face_vert_id = 0; face_vert_id < 4; face_vert_id++) {
                            mesh.vertices.emplace_back(
                                glm::vec3(block_pos + FACES[dir_id][face_vert_id]),
                                glm::vec3(DIRECTIONS[dir_id]),
                                glm::vec4(TEXTURE_UV[face_vert_id], tex_id, current_overlay),
                                tint
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