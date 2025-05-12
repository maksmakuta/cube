#include "cube/world/World.hpp"

namespace cube {

    World::World(const std::shared_ptr<IGenerator>& g) : m_generator(g) {}

    World::~World() = default;

}
