#include "cube/world/World.hpp"

#include <algorithm>
#include <iostream>

#include "cube/generators/GeneratorFlat.hpp"

namespace cube {

    World::World(const int seed) : m_generator(std::make_shared<GeneratorFlat>()), m_seed(seed) {}

    World::~World() = default;

    void World::setGenerator(const std::shared_ptr<IGenerator>& g) {
        m_generator = g;
        m_generator->setSeed(m_seed);
    }

    std::shared_ptr<IGenerator> World::getGenerator() const {
        return m_generator;
    }

    void World::insert(const Chunk& c) {
        const auto it = std::ranges::find_if(m_chunks,[&c](const Chunk& t) {
            return t.getOffset() == c.getOffset();
        });
        if (it != m_chunks.end()) {
            *it = c;
        }else {
            m_chunks.push_back(c);
        }
    }

    Chunk World::at(const glm::vec2 &pos) const {
        const auto it = std::ranges::find_if(m_chunks,[&pos](const Chunk& c) {
            return c.getOffset() == pos;
        });
        return it != m_chunks.end() ? *it : Chunk(pos);
    }

    void World::remove(const glm::vec2& pos) {
        std::erase_if(m_chunks,[&pos](const Chunk& c) {
            return c.getOffset() == pos;
        });
    }

    void World::onTick(const glm::vec3& pos) {
        std::cout << "World::onTick()" << std::endl;
    }

    int World::getSeed() const {
        return m_seed;
    }

    void World::setSeed(const int seed){
        m_seed = seed;
        if(m_generator) {
            m_generator->setSeed(seed);
        }
    }

}
