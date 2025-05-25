#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <glm/vec2.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "cube/core/IGenerator.hpp"
#include "cube/entities/Player.hpp"
#include "cube/utils/ThreadPool.hpp"
#include "cube/world/Chunk.hpp"

namespace cube {
    class World {
    public:
        World(int seed = 0);

        ~World();

        void onTick(ThreadPool &pool, const Player &player);

        [[nodiscard]] std::unordered_set<glm::ivec2> getVisibleChunks() const;

        [[nodiscard]] std::unordered_set<glm::ivec2> getChunks() const;

        ChunkPtr getChunk(const glm::ivec2 &p) const;

        int getSeed() const;

        void setSeed(int);

    private:
        std::unordered_map<glm::ivec2, ChunkPtr> m_chunks;
        std::unordered_set<glm::ivec2> m_visible;
        std::unique_ptr<IGenerator> m_generator;
    };
}

#endif //WORLD_HPP
