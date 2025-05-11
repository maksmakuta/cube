#ifndef WORLD_HPP
#define WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <unordered_map>

#include "cube/world/Chunk.hpp"

namespace cube {

    class World {
    public:
        World();
        ~World();

        Chunk getChunk(const glm::ivec2& pos);
        void setChunk(const glm::ivec2& pos, const Chunk& c);
        void removeChunk(const glm::ivec2& pos);

    private:
        std::unordered_map<glm::ivec2, Chunk> m_chunks{};
    };

}

#endif //WORLD_HPP
