#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include "Chunk.hpp"
#include "cube/core/IGenerator.hpp"

namespace cube {

    class World {
    public:
        explicit World(int seed = 0);
        ~World();

        void setGenerator(const std::shared_ptr<IGenerator>&);
        [[nodiscard]] std::shared_ptr<IGenerator> getGenerator() const;

        [[nodiscard]] int getSeed() const;
        void setSeed(int seed);

        void insert(const Chunk& c);
        Chunk& at(const glm::vec2& pos);
        void remove(const glm::vec2& pos);

        void onTick(const glm::vec3& pos);

    private:
        std::shared_ptr<IGenerator> m_generator;
        std::vector<Chunk> m_chunks;
        int m_seed;
    };

}

#endif //WORLD_HPP
