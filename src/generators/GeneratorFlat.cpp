#include <utility>

#include "cube/generators/GeneratorFlat.hpp"

namespace cube {
    FlatPreset FlatPreset::getPreset(const Preset p) {
        switch (p) {
            case Default:
                return {
                    {BlockID::HardStone, BlockID::Dirt, BlockID::Dirt, BlockID::Dirt, BlockID::Grass}
                };
            case Thin:
                return {
                    {BlockID::HardStone, BlockID::Grass}
                };
            case Thick:
                return {
                    {BlockID::HardStone, BlockID::Stone, BlockID::Stone, BlockID::Stone,
                             BlockID::Dirt, BlockID::Dirt, BlockID::Dirt, BlockID::Grass }
                };
            default:
                return {};
        }
    }

    GeneratorFlat::GeneratorFlat() : GeneratorFlat(FlatPreset::Default){}
    GeneratorFlat::GeneratorFlat(const FlatPreset::Preset p): m_preset(FlatPreset::getPreset(p)){}
    GeneratorFlat::GeneratorFlat(FlatPreset p) : m_preset(std::move(p)){}

    GeneratorFlat::~GeneratorFlat() = default;

    ChunkPtr GeneratorFlat::generateAt(const glm::vec2 &pos) {
        auto temp = std::make_shared<Chunk>();
        for (int y = 0; y < m_preset.layers.size(); ++y) {
            for (int z = 0; z < CHUNK_DEPTH; ++z) {
                for (int x = 0; x < CHUNK_WIDTH; ++x) {
                    temp->set({x,y,z},m_preset.layers[y]);
                }
            }
        }
        return temp;
    }

    void GeneratorFlat::postGenerate(const ChunkPtr& center,const std::array<ChunkPtr, 8> &neighbors,const glm::vec2 &pos) {
        // nothing to do here
    }


}