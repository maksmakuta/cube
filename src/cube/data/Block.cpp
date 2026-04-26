#include <cube/data/Block.hpp>
#include <array>

namespace cube {

    static constexpr std::array<BlockInfo, Block::COUNT> BLOCK_REGISTRY = {{
        {"Air",           {0,  0,  0,  0, -1, false, BlockShape::Cube}},

        // --- Solid Terrain ---
        {"GrassBlock",    {39, 21, 29, 1, 12, true,  BlockShape::Cube}},
        {"Dirt",          {29, 29, 29, 1, -1, false, BlockShape::Cube}},
        {"SnowDirt",      {16, 30, 29, 1, -1, false, BlockShape::Cube}},
        {"Snow",          {16, 16, 16, 1, -1, false, BlockShape::Cube}},
        {"Stone",         {71, 71, 71, 1, -1, false, BlockShape::Cube}},
        {"Sand",          {7,  7,  7,  1, -1, false, BlockShape::Cube}},
        {"Clay",          {37, 37, 37, 1, -1, false, BlockShape::Cube}},
        {"Gravel",        {48, 48, 48, 1, -1, false, BlockShape::Cube}},
        {"Diorite",       {20, 20, 20, 1, -1, false, BlockShape::Cube}},
        {"Tuff",          {35, 35, 35, 1, -1, false, BlockShape::Cube}},
        {"Ice",           {4,  4,  4,  1, -1, false, BlockShape::Cube}},

        // --- Ores ---
        {"OreCoal",       {46, 46, 46, 1, -1, false, BlockShape::Cube}},
        {"OreCopper",     {2,  2,  2,  1, -1, false, BlockShape::Cube}},
        {"OreIron",       {13, 13, 13, 1, -1, false, BlockShape::Cube}},
        {"OreGold",       {47, 47, 47, 1, -1, false, BlockShape::Cube}},
        {"CoralBubble",   {36, 36, 36, 1, -1, false, BlockShape::Cube}},

        // --- Wood & Leaves ---
        {"BirchLeaves",   {0,  0,  0,  1, -1, true,  BlockShape::Cube}},
        {"BirchLog",      {18, 9,  18, 1, -1, false, BlockShape::Cube}},
        {"JungleLeaves",  {22, 22, 22, 1, -1, true,  BlockShape::Cube}},
        {"JungleLog",     {40, 31, 40, 1, -1, false, BlockShape::Cube}},
        {"OakLeaves",     {32, 32, 32, 1, -1, true,  BlockShape::Cube}},
        {"OakLog",        {69, 41, 69, 1, -1, false, BlockShape::Cube}},
        {"SpruceLeaves",  {25, 25, 25, 1, -1, true,  BlockShape::Cube}},
        {"SpruceLog",     {43, 34, 43, 1, -1, false, BlockShape::Cube}},

        // --- Crops & Plants (Cubes) ---
        {"Melon",         {23, 14, 5,  1, -1, false, BlockShape::Cube}},
        {"Pumpkin",       {33, 24, 15, 1, -1, false, BlockShape::Cube}},
        {"Cactus",        {28, 19, 1,  1, -1, false, BlockShape::Cube}},

        // --- Cross-Shaped Flora ---
        {"CactusFlower",  {10, 10, 10, 1, -1, false, BlockShape::Cross}},
        {"Brown Mushroom",{27, 27, 27, 1, -1, false, BlockShape::Cross}},
        {"Red Mushroom",  {42, 42, 42, 1, -1, false, BlockShape::Cross}},
        {"Fern",          {38, 38, 38, 1, -1, true,  BlockShape::Cross}},
        {"Bush",          {45, 45, 45, 1, -1, true,  BlockShape::Cross}},
        {"Grass",         {3,  3,  3,  1, -1, true,  BlockShape::Cross}},
        {"Tallgrass",     {26, 17, 17, 1, -1, true,  BlockShape::Cross}},
        {"Sugarcane",     {8,  8,  8,  1, -1, false, BlockShape::Cross}},

        // Animated Cross-Shape
        {"Kelp",          {49, 49, 49, 20, -1, false, BlockShape::Cross}},

        {"Dandelion",     {11, 11, 11, 1, -1, false, BlockShape::Cross}},
        {"Poppy",         {6,  6,  6,  1, -1, false, BlockShape::Cross}},
        {"Tulip",         {70, 70, 70, 1, -1, false, BlockShape::Cross}},
        {"Vine",          {44, 44, 44, 1, -1, true,  BlockShape::Panel}},

        // --- Fluids ---
        {"Water",         {72, 72, 72, 36, -1, false, BlockShape::Liquid}}
    }};

    const BlockInfo& getBlockInfo(Block block) {
        return BLOCK_REGISTRY[block];
    }

    BlockData getBlockData(Block block) {
        return BLOCK_REGISTRY[block].data;
    }
}