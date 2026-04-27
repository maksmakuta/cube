#ifndef CUBE_BIOME_HPP
#define CUBE_BIOME_HPP

#include <cube/data/Block.hpp>

namespace cube {

    enum class BiomeType {
        //  Dry         Normal       Wet
        Tundra,     ColdSteppe,  Taiga,       // Cold
        Grassland,  Woodland,    Forest,      // Temperate
        Desert,     Savanna,     Rainforest   // Hot
    };

    struct BiomeTerrain final {
        Block terrain;
        Block subterrain;
        Block onwater;
        Block underwater1;
        Block underwater2;
    };

    BiomeType getBiome(float temp, float humidity);
    BiomeTerrain getTerrainData(BiomeType type);

}

#endif //CUBE_BIOME_HPP
