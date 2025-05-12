#ifndef GENERATORHEIGHT_HPP
#define GENERATORHEIGHT_HPP

#include "FastNoiseLite.h"
#include "cube/core/IGenerator.hpp"

namespace cube {

    class GeneratorHeight final : public IGenerator {
    public:
        explicit GeneratorHeight(int seed);
        ~GeneratorHeight() override;

        std::shared_ptr<Chunk> generateAt(const glm::vec2 &pos) override;
    private:
        FastNoiseLite noise;
    };

}


#endif //GENERATORHEIGHT_HPP
