#include <cube/data/Biome.hpp>

namespace cube {

    BiomeType getBiome(const float temp, const float humidity) {
        if (temp < -0.33f) {
            if (humidity < -0.33f) {
                return BiomeType::Tundra;
            }
            if (humidity > 0.33f) {
                return BiomeType::Taiga;
            }
            return BiomeType::ColdSteppe;
        }
        if (temp > 0.33f) {
            if (humidity < -0.33f) {
                return BiomeType::Desert;
            }
            if (humidity > 0.33f) {
                return BiomeType::Savanna;
            }
            return BiomeType::Rainforest;
        }

        if (humidity < -0.33f) {
            return BiomeType::Grassland;
        }
        if (humidity > 0.33f) {
            return BiomeType::Woodland;
        }
        return BiomeType::Forest;
    }

}