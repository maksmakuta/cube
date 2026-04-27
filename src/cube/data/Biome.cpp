#include <vector>
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

    const auto TERRAIN_DATA = std::vector<BiomeTerrain>{
        {SnowDirt,  Dirt,   Ice,    Dirt,       Dirt      },   //Tundra,
        {SnowDirt,  Dirt,   Ice,    Gravel,     Gravel    },   //ColdSteppe,
        {SnowDirt,  Dirt,   Ice,    Gravel,     Gravel    },   //Taiga,
        {GrassBlock,      Dirt,   Water,  Dirt,       Dirt      },   //Grassland,
        {GrassBlock,      Dirt,   Water,  Sand,       Clay      },   //Woodland,
        {GrassBlock,      Dirt,   Water,  Gravel,     Gravel    },   //Forest,
        {Sand,      Sand,   Water,  Sand,       Sand      },   //Desert,
        {GrassBlock,      Dirt,   Water,  Sand,       Clay      },   //Savanna,
        {GrassBlock,      Dirt,   Water,  Gravel,     Sand      },   //Rainforest
    };

    BiomeTerrain getTerrainData(const BiomeType type) {
        return TERRAIN_DATA[static_cast<int>(type)];
    }

}