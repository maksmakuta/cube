#ifndef IGENERATOR_HPP
#define IGENERATOR_HPP

#include "cube/world/Chunk.hpp"

namespace cube {

    class IGenerator {
    public:
        explicit IGenerator(const int seed = 0) : m_seed(seed){}
        virtual ~IGenerator() = default;

        virtual ChunkPtr generateAt(const glm::vec2& pos) = 0;

        [[nodiscard]] int getSeed() const {
            return m_seed;
        }

        void setSeed(const int seed){
            m_seed = seed;
        }

    private:
        int m_seed{0};
    };

}

#endif //IGENERATOR_HPP
