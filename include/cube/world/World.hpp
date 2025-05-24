#ifndef WORLD_HPP
#define WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <functional>
#include <memory>
#include <shared_mutex>

#include "cube/core/IGenerator.hpp"
#include "cube/utils/ThreadPool.hpp"

namespace cube {

    struct NewChunk {
        glm::ivec2 offset;
        ChunkPtr chunk;
    };

    class World {
        using ChunkPair = std::pair<ChunkPtr, bool>;
    public:
        explicit World(int seed = 0);
        ~World();

        [[nodiscard]] std::shared_ptr<IGenerator> getGenerator() const;
        [[nodiscard]] int getSeed() const;

        void setGenerator(const std::shared_ptr<IGenerator>&);
        void setSeed(int seed);

        ChunkPtr at(const glm::ivec2&);

        void onTick(ThreadPool& pool,const glm::vec3& player_pos);
        void forChunk(const std::function<void(const ChunkPtr&, const glm::ivec2&)>& fn);

    private:
        std::shared_mutex m_mutex;
        std::unordered_map<glm::ivec2,ChunkPair> m_chunks;
        std::shared_ptr<IGenerator> m_generator;
        int m_seed;
    };

}

#endif //WORLD_HPP
