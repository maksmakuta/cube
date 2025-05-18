#ifndef WORLD_HPP
#define WORLD_HPP

#include <functional>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

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

        void insert(const Chunk& c);
        Chunk at(const glm::ivec2& pos) const;
        void remove(const glm::ivec2& pos);

        void onTick(ThreadPool& pool,const glm::vec3& pos);
        void forChunk(const std::function<void(Chunk&)>& fn);

    private:
        std::unordered_map<glm::ivec2,Chunk> m_chunks;
        std::shared_ptr<IGenerator> m_generator;
        std::mutex m_chunks_mutex;
        int m_seed;
    };

}

#endif //WORLD_HPP
