#include "cube/generators/GeneratorTerrain.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "cube/utils/Utils.hpp"
#include "FastNoise/FastNoise.h"

namespace cube {

    GeneratorTerrain::GeneratorTerrain() : GeneratorTerrain(0){}

    GeneratorTerrain::GeneratorTerrain(const int seed) : IGenerator(seed) {
        const auto noise = FastNoise::New<FastNoise::Perlin>();
        generator = FastNoise::New<FastNoise::FractalFBm>();
        generator->SetSource(noise);
    }

    GeneratorTerrain::~GeneratorTerrain() = default;

    ChunkPtr GeneratorTerrain::generateAt(const glm::vec2 &pos) {
        auto chunk = std::make_shared<Chunk>();
        const auto delta = pos * glm::vec2{CHUNK_WIDTH,CHUNK_DEPTH};

        for (int x = 0; x < CHUNK_WIDTH; x++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                const auto n = generator->GenSingle2D((delta.x + x) * 0.01f,(delta.y + z) * 0.01f, getSeed());
                const auto h = CHUNK_WATER + static_cast<int>(n * 32);
                if (h > 0){
                    for (int y = 0; y < h; y++) {
                        chunk->set({x,y,z},BlockID::Dirt);
                    }
                    chunk->set({x,h,z},BlockID::Grass);
                }
            }
        }
        return chunk;
    }

    void norm(glm::ivec3 &p, ChunkPtr &chunk, const std::array<ChunkPtr, 8> &neighbors) {
        int d = 0;
        if (p.x < 0) {
            d--;
            p.x += CHUNK_WIDTH;
            chunk = neighbors.at(3);
        } else if (p.x >= CHUNK_WIDTH) {
            d++;
            p.x -= CHUNK_WIDTH;
            chunk = neighbors.at(4);
        }
        if (p.z < 0) {
            p.z += CHUNK_DEPTH;
            if (d < 0) {
                chunk = neighbors.at(7);
            } else if (d > 0) {
                chunk = neighbors.at(5);
            } else {
                chunk = neighbors.at(6);
            }
        } else if (p.z >= CHUNK_DEPTH) {
            p.z -= CHUNK_DEPTH;
            if (d < 0) {
                chunk = neighbors.at(0);
            } else if (d > 0) {
                chunk = neighbors.at(2);
            } else {
                chunk = neighbors.at(1);
            }
        }
    }

    void GeneratorTerrain::postGenerate(const ChunkPtr &center, const std::array<ChunkPtr, 8> &neighbors,
                                        const glm::vec2 &pos) {
        const auto delta = pos * glm::vec2{CHUNK_WIDTH, CHUNK_DEPTH};

        for (int x = 0; x < CHUNK_WIDTH; x++) {
            for (int z = 0; z < CHUNK_DEPTH; z++) {
                const auto n = generator->GenSingle2D((delta.x + x) * 0.01f, (delta.y + z) * 0.01f, getSeed());
                const auto h = CHUNK_WATER + static_cast<int>(n * 32);

                if (h > CHUNK_WATER) {
                    const auto f = generator->GenSingle2D((delta.x + x) * 0.9f, (delta.y + z) * 0.9f, getSeed() + 1);
                    if (f >= 0.35f) {
                        center->set({x, h + 1, z}, BlockID::Flower);
                    } else if (f <= -0.25f) {
                        center->set({x, h + 1, z}, BlockID::Bush);
                    }

                    const auto t = generator->GenSingle2D((delta.x + x) * 0.39f, (delta.y + z) * 0.39f, getSeed() + 1);
                    if (t > 0.55f) {
                        for (int i = 0; i < 4; i++) {
                            center->set({x, h + 1 + i, z}, BlockID::Wood);
                            if (i >= 2) {
                                for (int dx = -2; dx <= 2; dx++) {
                                    for (int dz = -2; dz <= 2; dz++) {
                                        auto p = glm::ivec3{x + dx, h + 1 + i, z + dz};
                                        auto c = center;
                                        norm(p, c, neighbors);
                                        if (inChunk(p)) {
                                            c->set(p, BlockID::Leaves);
                                        }
                                    }
                                }
                            }
                        }
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dz = -1; dz <= 1; dz++) {
                                auto p = glm::ivec3{x + dx, h + 5, z + dz};
                                auto c = center;
                                norm(p, c, neighbors);
                                if (inChunk(p)) {
                                    c->set(p, BlockID::Leaves);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
