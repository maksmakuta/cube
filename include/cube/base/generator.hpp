#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "cube/core/types.hpp"

namespace cube {

    class IGenerator {
    public:
        IGenerator() = default;
        virtual ~IGenerator() = default;

        virtual void seed(const Seed&) = 0;
        virtual BlockData at(uint8_t x,uint8_t y,uint8_t z) = 0;
    };

}

#endif //GENERATOR_HPP
