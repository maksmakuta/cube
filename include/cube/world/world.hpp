#ifndef WORLD_HPP
#define WORLD_HPP

#include "cube/base/generator.hpp"
#include "cube/core/types.hpp"
#include "cube/world/chunk.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec2.hpp>
#include <glm/gtx/hash.hpp>

#include <optional>
#include <memory>
#include <unordered_map>

namespace cube {

    class World {
    public:
        explicit World(const Seed& seed = 0);

        void loadChunk(int x, int z);
        void unloadChunk(int x, int z);

        std::optional<Chunk> getChunk(int x, int z);
        void setChunk(int x, int z, const Chunk& chunk);
    private:
        std::unordered_map<glm::ivec2, Chunk> m_chunks;
        std::shared_ptr<IGenerator> generator;
    };

}

#endif //WORLD_HPP
