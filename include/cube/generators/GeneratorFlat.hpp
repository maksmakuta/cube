#ifndef GENERATORFLAT_HPP
#define GENERATORFLAT_HPP

#include "cube/core/IGenerator.hpp"

namespace cube {

    struct FlatPreset {
        std::vector<BlockID> layers;

        enum Preset {
            Default,
            Thin,
            Thick
        };

        static FlatPreset getPreset(Preset);
    };

    class GeneratorFlat final : public IGenerator {
    public:
        GeneratorFlat();
        explicit GeneratorFlat(FlatPreset::Preset);
        explicit GeneratorFlat(FlatPreset );
        ~GeneratorFlat() override;

        Chunk generateAt(const glm::vec2 &pos) override;

    private:
        FlatPreset m_preset;
    };

}

#endif //GENERATORFLAT_HPP
