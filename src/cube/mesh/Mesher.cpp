#include "cube/mesh/Mesher.hpp"

#include "cube/utils/Log.hpp"

namespace cube {

    const auto DIRECTIONS = std::vector<glm::ivec3>{
        {1,0,0},
        {-1,0,0},
        {0,1,0},
        {0,-1,0},
        {0,0,1},
        {0,0,-1}
    };

    const auto TEXTURE_UV = std::vector<glm::ivec2>{
        {0,0},
        {0,1},
        {1,1},
        {1,0},
    };

    //TODO define FACES data
    const auto FACES = std::vector<std::vector<glm::ivec3>>{
        {{},{},{},{}},
        {{},{},{},{}},
        {{},{},{},{}},
        {{},{},{},{}},
        {{},{},{},{}},
        {{},{},{},{}},
    };

    inline bool outOfPlane(const int val) {
        return val < 0 || val >= CHUNK_SIZE;
    }

    inline bool isOutside(const glm::ivec3& pos) {
        return outOfPlane(pos.x) || outOfPlane(pos.y) || outOfPlane(pos.z);
    }

    ChunkMesh mesh(const glm::ivec3& pos, const World& world) {
        const auto chunk = world.getChunk(pos);
        if (!chunk) {
            warn("Cannot load chunk [{},{},{}]", pos.x, pos.y, pos.z);
            return {};
        }

        ChunkMesh mesh{};

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    const auto block_pos = glm::ivec3(x, y, z);

                    for (auto dir_id = 0; dir_id < DIRECTIONS.size(); dir_id++) {
                        const auto n_block_pos = block_pos + DIRECTIONS[dir_id];
                        if (isOutside(n_block_pos)) {
                            continue;
                        }
                        if (chunk->at(n_block_pos) != Block::Air) {
                            continue;
                        }

                        const auto index = mesh.vertices.size();
                        for (auto face_id = 0; face_id < 4; face_id++) {
                            mesh.vertices.emplace_back(
                                block_pos + FACES[dir_id][face_id],
                                DIRECTIONS[dir_id],
                                TEXTURE_UV[face_id],
                                0 //TODO(implement block data with all predefined textures data)
                            );
                        }

                        for (const auto& indice : {0,1,2,1,2,3}) {
                            mesh.indices.push_back(index + indice);
                        }

                    }

                }
            }
        }

        return mesh;
    }

}
