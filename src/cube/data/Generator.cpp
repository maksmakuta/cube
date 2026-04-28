#include "cube/data/Generator.hpp"

namespace cube {

    void generateChunk(const glm::ivec3& pos, World& world, ThreadPool& pool) {

        pool.enqueue([&pos, &world] {
            auto chunk = std::make_unique<Chunk>();

            const auto chunk_y = pos.y * CHUNK_SIZE.y;

            for (int x = 0; x < CHUNK_SIZE.x; x++) {
                for (int z = 0; z < CHUNK_SIZE.z; z++) {
                    for (int y = 0; y < CHUNK_SIZE.y; y++) {
                        const auto world_y = chunk_y + y;
                        if (world_y > 32) {
                            (*chunk)[x,y,z] = Block::Air;
                        }if (world_y == 32) {
                            (*chunk)[x,y,z] = Block::Grass;
                        }else if (world_y >= 24) {
                            (*chunk)[x,y,z] = Block::Dirt;
                        }else if (world_y == 0) {
                            (*chunk)[x,y,z] = Block::Bedrock;
                        }else {
                            (*chunk)[x,y,z] = Block::Stone;
                        }
                    }
                }
            }

            world.insertChunk(pos, std::move(chunk));
        });
    }

}