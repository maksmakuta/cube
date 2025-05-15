#ifndef GENERATORHEIGHT_HPP
#define GENERATORHEIGHT_HPP

#include "cube/core/IGenerator.hpp"
#include "cube/utils/FNoise.hpp"

namespace cube {

    class GeneratorHeight final : public IGenerator {
    public:
        explicit GeneratorHeight(int seed);
        ~GeneratorHeight() override;

        Chunk generateAt(const glm::vec2 &pos) override;
    private:
        FNoise noise;
    };

}


#endif //GENERATORHEIGHT_HPP
