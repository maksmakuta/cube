#ifndef IGENERATOR_HPP
#define IGENERATOR_HPP
#include <glm/vec2.hpp>

#include "cube/world/Chunk.hpp"

namespace cube {
    enum class GeneratorFeature {
        Plants,
        //TODO(add more features)
    };

    enum class Biomes {
        Sea,
        Beach,
        Plains,
        Forest,
        Mountains
    };

    class IGenerator {
    public:
        explicit IGenerator(const int seed) : m_seed(seed) {
        }

        virtual ~IGenerator() = default;

        virtual int getHeight(int x, int z) = 0;

        virtual float getProbability(int x, int z, GeneratorFeature feature) = 0;

        virtual ChunkPtr getChunk(const glm::ivec2 &pos) = 0;

        [[nodiscard]] int getSeed() const {
            return m_seed;
        }

        void setSeed(const int seed) {
            m_seed = seed;
        }

    private:
        int m_seed;
    };
}

#endif //IGENERATOR_HPP
