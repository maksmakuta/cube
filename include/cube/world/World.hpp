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

    class World {
    public:
        explicit World(int seed = 0);
        ~World();

        void setGenerator(const std::shared_ptr<IGenerator>&);
        [[nodiscard]] std::shared_ptr<IGenerator> getGenerator() const;

        [[nodiscard]] int getSeed() const;
        void setSeed(int seed);

        Chunk& at(const glm::ivec2&);

        void onTick(ThreadPool& pool,const glm::vec3& pos);
        void forChunk(const std::function<void(const glm::ivec2&)>& fn);

    private:
        std::unordered_map<glm::ivec2,Chunk> m_chunks;
        std::shared_ptr<IGenerator> m_generator;
        std::queue<Chunk> m_new_chunks;
        std::mutex m_chunks_mutex;
        std::shared_mutex m_read_mutex;
        int m_seed;
    };

}

#endif //WORLD_HPP
