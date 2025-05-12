#ifndef WORLD_HPP
#define WORLD_HPP

#include "cube/core/IGenerator.hpp"
#include "cube/generators/GeneratorFlat.hpp"

namespace cube {

    class World {
    public:
        explicit World(const std::shared_ptr<IGenerator>& = std::make_shared<GeneratorFlat>());
        ~World();
    private:
        std::shared_ptr<IGenerator> m_generator;
    };

}

#endif //WORLD_HPP
