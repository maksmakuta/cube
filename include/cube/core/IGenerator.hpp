#ifndef IGENERATOR_HPP
#define IGENERATOR_HPP

#include <glm/vec2.hpp>

#include "cube/world/Chunk.hpp"

namespace cube {

    class IGenerator {
    public:
        virtual ~IGenerator() = default;

        virtual Chunk generateAt(const glm::vec2& pos) = 0;
    };

}

#endif //IGENERATOR_HPP
