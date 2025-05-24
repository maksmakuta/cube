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

        ChunkPtr generateAt(const glm::vec2 &pos) override;
        void postGenerate(const ChunkPtr &center, const std::array<ChunkPtr, 8> &neighbors,const glm::vec2 &pos) override;

    private:
        FlatPreset m_preset;
    };

}

#endif //GENERATORFLAT_HPP
