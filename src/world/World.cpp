#include "cube/world/World.hpp"

#include "cube/utils/Utils.hpp"

namespace cube {

    World::World(const std::shared_ptr<IGenerator>& g) : m_generator(g) {}

    World::~World() = default;

    void World::onTick(const glm::vec3& pos) {
        const auto chunk_pos = toChunk(pos);

        for (int x = -RENDER_DIST; x <= RENDER_DIST; ++x) {
            for (int y = -RENDER_DIST; y <= RENDER_DIST; ++y) {

            }
        }
    }

}
