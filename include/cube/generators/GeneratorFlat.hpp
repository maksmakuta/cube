#ifndef GENERATORFLAT_HPP
#define GENERATORFLAT_HPP

#include "cube/core/IGenerator.hpp"

namespace cube {

    class GeneratorFlat final : public IGenerator {
    public:
        GeneratorFlat();
        ~GeneratorFlat() override;

        Chunk generateAt(const glm::vec2 &pos) override;

    };

}

#endif //GENERATORFLAT_HPP
