#ifndef CUBE_BIOME_HPP
#define CUBE_BIOME_HPP

namespace cube {

    enum class BiomeType {
        //  Dry         Normal       Wet
        Tundra,     ColdSteppe,  Taiga,       // Cold
        Grassland,  Woodland,    Forest,      // Temperate
        Desert,     Savanna,     Rainforest   // Hot
    };

    BiomeType getBiome(float temp, float humidity);

}

#endif //CUBE_BIOME_HPP
