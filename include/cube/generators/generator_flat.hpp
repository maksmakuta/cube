#ifndef GENERATOR_FLAT_HPP
#define GENERATOR_FLAT_HPP

#include "cube/base/generator.hpp"

namespace cube::generators {

    class GeneratorFlat final : public IGenerator{
    public:
        GeneratorFlat();

        void seed(const Seed&) override;
        BlockData at(uint8_t x, uint8_t y, uint8_t z) override;
    };

}

#endif //GENERATOR_FLAT_HPP
