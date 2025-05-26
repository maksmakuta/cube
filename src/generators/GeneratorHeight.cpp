#include "cube/generators/GeneratorHeight.hpp"

#include "cube/core/Constants.hpp"

namespace cube {
    GeneratorHeight::GeneratorHeight(const int seed) : IGenerator(seed) {
        using namespace FastNoise;

        const auto base = New<FractalFBm>();
        base->SetSource(New<Perlin>());
        base->SetOctaveCount(3);

        const auto detail = New<FractalFBm>();
        detail->SetSource(New<Simplex>());
        detail->SetOctaveCount(2);

        const auto add = New<Add>();
        add->SetLHS(base);
        add->SetRHS(detail);

        m_noise = add;
    }

    GeneratorHeight::~GeneratorHeight() = default;

    ChunkPtr GeneratorHeight::getChunk(const glm::ivec2 &pos) {
        const auto new_chunk = std::make_shared<Chunk>();

        for (int x = 0; x < CHUNK_WIDTH; ++x) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                const auto h = getHeight(pos.x * CHUNK_WIDTH + x, pos.y * CHUNK_DEPTH + z);
                for (int y = 0; y < h; ++y) {
                    BlockID b;
                    if (y == 0) {
                        b = BlockID::HardStone;
                    } else if (y >= h - 3) {
                        b = BlockID::Dirt;
                    } else {
                        b = BlockID::Stone;
                    }
                    new_chunk->setBlock(x, y, z, b);
                }
                new_chunk->setBlock(x, h, z, BlockID::Grass);
            }
        }

        return new_chunk;
    }

    int GeneratorHeight::getHeight(const int x, const int z) {
        const auto fx = static_cast<float>(x) * 0.01f;
        const auto fz = static_cast<float>(z) * 0.01f;
        return 64 + static_cast<int>(m_noise->GenSingle2D(fx, fz, getSeed()) * 24);
    }

    float GeneratorHeight::getProbability(int x, int z, GeneratorFeature feature) {
        return 0.f;
    }
}
