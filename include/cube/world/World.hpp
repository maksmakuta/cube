#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

#include "cube/core/IGenerator.hpp"
#include "cube/generators/GeneratorFlat.hpp"

namespace cube {

    class World {
    public:
        explicit World(
            const std::shared_ptr<IGenerator>& = std::make_shared<GeneratorFlat>()
        );
        ~World();

        void onTick(const glm::vec3& pos);
    private:
        std::shared_ptr<IGenerator> m_generator;
        std::vector<Chunk> m_chunks;
    };

}

#endif //WORLD_HPP
