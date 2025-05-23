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
                const auto h = 48 + static_cast<int>(n * 32);
                if (h > 0){
                    for (int y = 0; y < h; y++) {
                        chunk->set({x,y,z},BlockID::Dirt);
                    }
                    chunk->set({x,h,z},BlockID::Grass);
                }

                const auto f = generator->GenSingle2D((delta.x + x) * 0.9f,(delta.y + z) * 0.9f, getSeed() + 1);
                if (f >= 0.5f) {
                    chunk->set({x,h + 1 ,z},BlockID::Flower);
                }else if (f <= -0.5f) {
                    chunk->set({x,h + 1 ,z},BlockID::Bush);
                }

                const auto t = generator->GenSingle2D((delta.x + x) * 0.69f,(delta.y + z) * 0.69f, getSeed() - 1);
                if (t >= 0.5f) {
                    chunk->set({x,h + 1 ,z},BlockID::Wood);
                    //TODO( generate normal trees)
                }
            }
        }
        return chunk;
    }

}
