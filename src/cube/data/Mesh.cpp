#include "cube/data/Mesh.hpp"

#include <array>

#include "cube/core/BlockID.hpp"
#include "cube/data/Chunk.hpp"
#include "cube/data/World.hpp"

namespace cube {

    constexpr int MASK_SIZE  = CHUNK_SIZE * CHUNK_SIZE;

    enum Face : uint8_t {
        Top,
        Bottom,
        Left,
        Right,
        Near,
        Far
    };

    struct TextureSet {
        uint8_t top;
        uint8_t side;
        uint8_t bottom;
    };

    constexpr TextureSet TEXTURES[] = {
        {  0,  0,  0 }, //Air,
        {  0,  0,  0 }, //SnowGrassBlock,
        {115,113,109 }, //GrassBlock,
        {  0,  0,  0 }, //CoarseDirt,
        {  0,  0,  0 }, //RedSand,
        {  0,  0,  0 }, //Gravel,
        {  0,  0,  0 }, //Clay,
        {109,109,109 }, //Dirt,
        {  0,  0,  0 }, //Sand,
        {  0,  0,  0 }, //Mud,
        {  0,  0,  0 }, //Amethyst,
        {  0,  0,  0 }, //Andesite,
        {  0,  0,  0 }, //Basalt,
        { 98, 98, 98 }, //Bedrock,
        {  0,  0,  0 }, //BlackStone,
        {  0,  0,  0 }, //Calcite,
        {  0,  0,  0 }, //CobbleStone,
        {  0,  0,  0 }, //Deepslate,
        {  0,  0,  0 }, //Diorite,
        {  0,  0,  0 }, //Dripstone,
        {  0,  0,  0 }, //Granite,
        {  0,  0,  0 }, //RedSandStone,
        {  0,  0,  0 }, //SandStone,
        {  0,  0,  0 }, //Stone,
        {  0,  0,  0 }, //Tuff,
        {  0,  0,  0 }, //Ice,
        {  0,  0,  0 }, //BlueIce,
        {  0,  0,  0 }, //PackedIce,
        {  0,  0,  0 }, //AcaciaLeaves,
        {  0,  0,  0 }, //AzaleaLeaves,
        {  0,  0,  0 }, //BirchLeaves,
        {  0,  0,  0 }, //CherryLeaves,
        {  0,  0,  0 }, //DarkOakLeaves,
        {  0,  0,  0 }, //JungleLeaves,
        {  0,  0,  0 }, //OakLeaves,
        {  0,  0,  0 }, //PaleOakLeaves,
        {  0,  0,  0 }, //SpruceLeaves,
        {  0,  0,  0 }, //AcaciaLog,
        {  0,  0,  0 }, //AzaleaLog,
        {  0,  0,  0 }, //BirchLog,
        {  0,  0,  0 }, //CherryLog,
        {  0,  0,  0 }, //DarkOakLog,
        {  0,  0,  0 }, //JungleLog,
        {  0,  0,  0 }, //OakLog,
        {  0,  0,  0 }, //PaleOakLog,
        {  0,  0,  0 }, //SpruceLog,
        {  0,  0,  0 }, //AcaciaSapling,
        {  0,  0,  0 }, //AzaleaSapling,
        {  0,  0,  0 }, //BirchSapling,
        {  0,  0,  0 }, //CherrySapling,
        {  0,  0,  0 }, //DarkOakSapling,
        {  0,  0,  0 }, //JungleSapling,
        {  0,  0,  0 }, //OakSapling,
        {  0,  0,  0 }, //PaleOakSapling,
        {  0,  0,  0 }, //SpruceSapling,
        {  0,  0,  0 }, //Allium,
        {  0,  0,  0 }, //AzureBluet,
        {  0,  0,  0 }, //BlueOrchid,
        {  0,  0,  0 }, //CornFlower,
        {  0,  0,  0 }, //Dandelion,
        {  0,  0,  0 }, //OrangeTulip,
        {  0,  0,  0 }, //OxeyeDaisy,
        {  0,  0,  0 }, //PinkTulip,
        {  0,  0,  0 }, //Poppy,
        {  0,  0,  0 }, //RedTulip,
        {  0,  0,  0 }, //WhiteTulip,
        {  0,  0,  0 }, //BrainCoralBlock,
        {  0,  0,  0 }, //BubbleCoralBlock,
        {  0,  0,  0 }, //FireCoralBlock,
        {  0,  0,  0 }, //HornCoralBlock,
        {  0,  0,  0 }, //TubeCoralBlock,
        {  0,  0,  0 }, //DeadBrainCoralBlock,
        {  0,  0,  0 }, //DeadBubbleCoralBlock,
        {  0,  0,  0 }, //DeadFireCoralBlock,
        {  0,  0,  0 }, //DeadHornCoralBlock,
        {  0,  0,  0 }, //DeadTubeCoralBlock,
        {  0,  0,  0 }, //BrainCoralFan,
        {  0,  0,  0 }, //BubbleCoralFan,
        {  0,  0,  0 }, //FireCoralFan,
        {  0,  0,  0 }, //HornCoralFan,
        {  0,  0,  0 }, //TubeCoralFan,
        {  0,  0,  0 }, //BrainCoral,
        {  0,  0,  0 }, //BubbleCoral,
        {  0,  0,  0 }, //FireCoral,
        {  0,  0,  0 }, //HornCoral,
        {  0,  0,  0 }, //TubeCoral,
        {  0,  0,  0 }, //DeadBrainCoral,
        {  0,  0,  0 }, //DeadBubbleCoral,
        {  0,  0,  0 }, //DeadFireCoral,
        {  0,  0,  0 }, //DeadHornCoral,
        {  0,  0,  0 }, //DeadTubeCoral,
        {  0,  0,  0 }, //Bush,
        {  0,  0,  0 }, //DeadBush,
        {  0,  0,  0 }, //Fern,
        {  0,  0,  0 }, //Grass,
        {  0,  0,  0 }, //HangingRoots,
        {  0,  0,  0 }, //ShortDryGrass,
        {  0,  0,  0 }, //ShortGrass,
        {  0,  0,  0 }, //TallDryGrass,
        {  0,  0,  0 }, //SugarCane,
        {  0,  0,  0 }, //SweetBerry0,
        {  0,  0,  0 }, //SweetBerry1,
        {  0,  0,  0 }, //SweetBerry2,
        {  0,  0,  0 }, //SweetBerry3,
        {  0,  0,  0 }, //CaveVines,
        {  0,  0,  0 }, //CaveVinesLit,
        {  0,  0,  0 }, //CavePlant,
        {  0,  0,  0 }, //CavePlantLit,
        {  0,  0,  0 }, //Cactus,
        {  0,  0,  0 }, //CactusFlower,
        {  0,  0,  0 }, //BrownMushroom,
        {  0,  0,  0 }, //RedMushroom,
        {  0,  0,  0 }, //Pumpkin,
        {  0,  0,  0 }, //Melon,
        {  0,  0,  0 }, //LargeFern,
        {  0,  0,  0 }, //LargeFernTop,
        {  0,  0,  0 }, //Peony,
        {  0,  0,  0 }, //PeonyTop,
        {  0,  0,  0 }, //RoseBush,
        {  0,  0,  0 }, //RoseBushTop,
        {  0,  0,  0 }, //TallGrass,
        {  0,  0,  0 }, //TallGrassTop,
        {  0,  0,  0 }, //TallSeaGrass,
        {  0,  0,  0 }, //TallSeaGrassTop,
        {  0,  0,  0 }, //Kelp,
        {  0,  0,  0 }, //Seagrass,
        {  0,  0,  0 }, //Water,
        {  0,  0,  0 }, //Lava,
        {  0,  0,  0 }, //COUNT
    };

    constexpr bool TRANSPARENT[] = {
        true,  //Air,
        false, //SnowGrassBlock,
        false, //GrassBlock,
        false, //CoarseDirt,
        false, //RedSand,
        false, //Gravel,
        false, //Clay,
        false, //Dirt,
        false, //Sand,
        false, //Mud,
        false, //Amethyst,
        false, //Andesite,
        false, //Basalt,
        false, //Bedrock,
        false, //BlackStone,
        false, //Calcite,
        false, //CobbleStone,
        false, //Deepslate,
        false, //Diorite,
        false, //Dripstone,
        false, //Granite,
        false, //RedSandStone,
        false, //SandStone,
        false, //Stone,
        false, //Tuff,
        false, //Ice,
        false, //BlueIce,
        false, //PackedIce,
        false, //AcaciaLeaves,
        false, //AzaleaLeaves,
        false, //BirchLeaves,
        false, //CherryLeaves,
        false, //DarkOakLeaves,
        false, //JungleLeaves,
        false, //OakLeaves,
        false, //PaleOakLeaves,
        false, //SpruceLeaves,
        false, //AcaciaLog,
        false, //AzaleaLog,
        false, //BirchLog,
        false, //CherryLog,
        false, //DarkOakLog,
        false, //JungleLog,
        false, //OakLog,
        false, //PaleOakLog,
        false, //SpruceLog,
        false, //AcaciaSapling,
        false, //AzaleaSapling,
        false, //BirchSapling,
        false, //CherrySapling,
        false, //DarkOakSapling,
        false, //JungleSapling,
        false, //OakSapling,
        false, //PaleOakSapling,
        false, //SpruceSapling,
        false, //Allium,
        false, //AzureBluet,
        false, //BlueOrchid,
        false, //CornFlower,
        false, //Dandelion,
        false, //OrangeTulip,
        false, //OxeyeDaisy,
        false, //PinkTulip,
        false, //Poppy,
        false, //RedTulip,
        false, //WhiteTulip,
        false, //BrainCoralBlock,
        false, //BubbleCoralBlock,
        false, //FireCoralBlock,
        false, //HornCoralBlock,
        false, //TubeCoralBlock,
        false, //DeadBrainCoralBlock,
        false, //DeadBubbleCoralBlock,
        false, //DeadFireCoralBlock,
        false, //DeadHornCoralBlock,
        false, //DeadTubeCoralBlock,
        false, //BrainCoralFan,
        false, //BubbleCoralFan,
        false, //FireCoralFan,
        false, //HornCoralFan,
        false, //TubeCoralFan,
        false, //BrainCoral,
        false, //BubbleCoral,
        false, //FireCoral,
        false, //HornCoral,
        false, //TubeCoral,
        false, //DeadBrainCoral,
        false, //DeadBubbleCoral,
        false, //DeadFireCoral,
        false, //DeadHornCoral,
        false, //DeadTubeCoral,
        false, //Bush,
        false, //DeadBush,
        false, //Fern,
        false, //Grass,
        false, //HangingRoots,
        false, //ShortDryGrass,
        false, //ShortGrass,
        false, //TallDryGrass,
        false, //SugarCane,
        false, //SweetBerry0,
        false, //SweetBerry1,
        false, //SweetBerry2,
        false, //SweetBerry3,
        false, //CaveVines,
        false, //CaveVinesLit,
        false, //CavePlant,
        false, //CavePlantLit,
        false, //Cactus,
        false, //CactusFlower,
        false, //BrownMushroom,
        false, //RedMushroom,
        false, //Pumpkin,
        false, //Melon,
        false, //LargeFern,
        false, //LargeFernTop,
        false, //Peony,
        false, //PeonyTop,
        false, //RoseBush,
        false, //RoseBushTop,
        false, //TallGrass,
        false, //TallGrassTop,
        false, //TallSeaGrass,
        false, //TallSeaGrassTop,
        false, //Kelp,
        false, //Seagrass,
        false, //Water,
        false, //Lava,
        false, //COUNT
    };

    [[nodiscard]]
    inline uint8_t getTexture(const BlockID id, const Face face) noexcept {
        const auto& t = TEXTURES[static_cast<uint8_t>(id)];

        switch (face) {
            case Top:
                return t.top;

            case Bottom:
                return t.bottom;

            default:
                return t.side;
        }
    }

    [[nodiscard]]
    inline bool isTransparent(const BlockID id) noexcept {
        return TRANSPARENT[static_cast<uint8_t>(id)];
    }

    struct ChunkCache final {
        Chunk* chunks[3][3][3] = {};
    };

    [[nodiscard]]
    inline BlockID voxel(
        const ChunkCache& cache,
        const int x,
        const int y,
        const int z
    ) noexcept {

        const int cx = x >> 4;
        const int cy = y >> 4;
        const int cz = z >> 4;

        const int lx = x & 15;
        const int ly = y & 15;
        const int lz = z & 15;

        Chunk* chunk = cache.chunks[cx + 1][cy + 1][cz + 1];

        if (!chunk) {
            return BlockID::Air;
        }

        return static_cast<BlockID>(
            chunk->at({lx, ly, lz}) & 0xFF
        );
    }

    struct FaceInfo final {
        BlockID id = BlockID::Air;
        uint8_t texture = 0;
        uint8_t ao = 0;

        [[nodiscard]]
        bool operator==(const FaceInfo& other) const noexcept {
            return id == other.id &&
                   texture == other.texture &&
                   ao == other.ao;
        }
    };

    [[nodiscard]]
    inline uint8_t packAO(
        const uint8_t a0,
        const uint8_t a1,
        const uint8_t a2,
        const uint8_t a3
    ) noexcept {

        return (a0) |
            (a1 << 2) |
            (a2 << 4) |
            (a3 << 6);
    }

    [[nodiscard]]
    inline uint8_t unpackAO(
        const uint8_t ao,
        const int index
    ) noexcept {

        return (ao >> (index * 2)) & 0x3;
    }

    [[nodiscard]]
    inline uint8_t computeAO(
        const bool s1,
        const bool s2,
        const bool c
    ) noexcept {

        if (s1 && s2) {
            return 0;
        }

        return 3 - (s1 + s2 + c);
    }

    Mesh generateMesh(const glm::ivec3& pos, const World& world) {
        Mesh mesh;

        mesh.solid.vertices.reserve(32768);
        mesh.solid.indices.reserve(49152);

        mesh.transparent.vertices.reserve(4096);
        mesh.transparent.indices.reserve(6144);

        ChunkCache cache;

        for (int z = -1; z <= 1; ++z) {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {

                    auto ptr = world.getChunk(
                        pos + glm::ivec3(x, y, z)
                    );

                    cache.chunks[x + 1][y + 1][z + 1] = ptr.get();
                }
            }
        }

        constexpr int normals[6][3] = {
            { 0,  1,  0},
            { 0, -1,  0},
            {-1,  0,  0},
            { 1,  0,  0},
            { 0,  0,  1},
            { 0,  0, -1}
        };

        constexpr int axisU[6] = {2, 2, 2, 2, 0, 0};
        constexpr int axisV[6] = {0, 0, 1, 1, 1, 1};
        constexpr int axisD[6] = {1, 1, 0, 0, 2, 2};

        constexpr int aoOffsets[6][4][3][3] = {

            // Top
            {
                {{ 0, 0,-1}, {-1, 0, 0}, {-1, 0,-1}},
                {{ 0, 0,-1}, { 1, 0, 0}, { 1, 0,-1}},
                {{ 0, 0, 1}, {-1, 0, 0}, {-1, 0, 1}},
                {{ 0, 0, 1}, { 1, 0, 0}, { 1, 0, 1}}
            },

            // Bottom
            {
                {{ 0, 0,-1}, {-1, 0, 0}, {-1, 0,-1}},
                {{ 0, 0,-1}, { 1, 0, 0}, { 1, 0,-1}},
                {{ 0, 0, 1}, {-1, 0, 0}, {-1, 0, 1}},
                {{ 0, 0, 1}, { 1, 0, 0}, { 1, 0, 1}}
            },

            // Left
            {
                {{ 0, 0,-1}, { 0,-1, 0}, { 0,-1,-1}},
                {{ 0, 0,-1}, { 0, 1, 0}, { 0, 1,-1}},
                {{ 0, 0, 1}, { 0,-1, 0}, { 0,-1, 1}},
                {{ 0, 0, 1}, { 0, 1, 0}, { 0, 1, 1}}
            },

            // Right
            {
                {{ 0, 0,-1}, { 0,-1, 0}, { 0,-1,-1}},
                {{ 0, 0,-1}, { 0, 1, 0}, { 0, 1,-1}},
                {{ 0, 0, 1}, { 0,-1, 0}, { 0,-1, 1}},
                {{ 0, 0, 1}, { 0, 1, 0}, { 0, 1, 1}}
            },

            // Near
            {
                {{-1, 0, 0}, { 0,-1, 0}, {-1,-1, 0}},
                {{-1, 0, 0}, { 0, 1, 0}, {-1, 1, 0}},
                {{ 1, 0, 0}, { 0,-1, 0}, { 1,-1, 0}},
                {{ 1, 0, 0}, { 0, 1, 0}, { 1, 1, 0}}
            },

            // Far
            {
                {{-1, 0, 0}, { 0,-1, 0}, {-1,-1, 0}},
                {{-1, 0, 0}, { 0, 1, 0}, {-1, 1, 0}},
                {{ 1, 0, 0}, { 0,-1, 0}, { 1,-1, 0}},
                {{ 1, 0, 0}, { 0, 1, 0}, { 1, 1, 0}}
            }
        };

        std::array<FaceInfo, MASK_SIZE> mask {};

        uint32_t solidOffset = 0;
        uint32_t blendOffset = 0;

        int x[3] {};

        for (int f = 0; f < 6; ++f) {

            const Face face = static_cast<Face>(f);

            const int nx = normals[f][0];
            const int ny = normals[f][1];
            const int nz = normals[f][2];

            const int u = axisU[f];
            const int v = axisV[f];
            const int d = axisD[f];

            for (x[d] = 0; x[d] < CHUNK_SIZE; ++x[d]) {

                mask.fill({});

                int n = 0;

                for (x[v] = 0; x[v] < CHUNK_SIZE; ++x[v]) {
                    for (x[u] = 0; x[u] < CHUNK_SIZE; ++x[u], ++n) {

                        const int vx = x[0];
                        const int vy = x[1];
                        const int vz = x[2];

                        const BlockID current =
                            voxel(cache, vx, vy, vz);

                        if (current == BlockID::Air) {
                            continue;
                        }

                        const BlockID neighbor =
                            voxel(
                                cache,
                                vx + nx,
                                vy + ny,
                                vz + nz
                            );

                        const bool curTrans =
                            isTransparent(current);

                        const bool neiTrans =
                            isTransparent(neighbor);

                        const bool visible =
                            neighbor == BlockID::Air ||
                            (neiTrans && !curTrans) ||
                            (curTrans && neiTrans && current != neighbor);

                        if (!visible) {
                            continue;
                        }

                        FaceInfo& faceInfo = mask[n];

                        faceInfo.id = current;
                        faceInfo.texture = getTexture(current, face);

                        uint8_t ao[4];

                        for (int i = 0; i < 4; ++i) {

                            const auto& side1 = aoOffsets[f][i][0];
                            const auto& side2 = aoOffsets[f][i][1];
                            const auto& corner = aoOffsets[f][i][2];

                            const bool s1 =
                                voxel(
                                    cache,
                                    vx + nx + side1[0],
                                    vy + ny + side1[1],
                                    vz + nz + side1[2]
                                ) != BlockID::Air;

                            const bool s2 =
                                voxel(
                                    cache,
                                    vx + nx + side2[0],
                                    vy + ny + side2[1],
                                    vz + nz + side2[2]
                                ) != BlockID::Air;

                            const bool c =
                                voxel(
                                    cache,
                                    vx + nx + corner[0],
                                    vy + ny + corner[1],
                                    vz + nz + corner[2]
                                ) != BlockID::Air;

                            ao[i] = computeAO(s1, s2, c);
                        }

                        faceInfo.ao =
                            packAO(ao[0], ao[1], ao[2], ao[3]);
                    }
                }

                n = 0;

                for (int j = 0; j < CHUNK_SIZE; ++j) {
                    for (int i = 0; i < CHUNK_SIZE;) {

                        const FaceInfo current = mask[n];

                        if (current.id == BlockID::Air) {
                            ++i;
                            ++n;
                            continue;
                        }

                        int w = 1;

                        while (
                            i + w < CHUNK_SIZE &&
                            mask[n + w] == current
                        ) {
                            ++w;
                        }

                        int h = 1;
                        bool done = false;

                        while (j + h < CHUNK_SIZE && !done) {

                            for (int k = 0; k < w; ++k) {
                                if (!(mask[n + k + h * CHUNK_SIZE] == current)) {
                                    done = true;
                                    break;
                                }
                            }

                            if (!done) {
                                ++h;
                            }
                        }

                        x[u] = i;
                        x[v] = j;

                        int du[3] {};
                        int dv[3] {};

                        du[u] = w;
                        dv[v] = h;

                        glm::ivec3 v0;
                        glm::ivec3 v1;
                        glm::ivec3 v2;
                        glm::ivec3 v3;

                        if (f == Top || f == Right || f == Near) {

                            v0 = {x[0] + nx,               x[1] + ny,               x[2] + nz};
                            v1 = {x[0] + nx + du[0],       x[1] + ny + du[1],       x[2] + nz + du[2]};
                            v2 = {x[0] + nx + dv[0],       x[1] + ny + dv[1],       x[2] + nz + dv[2]};
                            v3 = {x[0] + nx + du[0] + dv[0],
                                  x[1] + ny + du[1] + dv[1],
                                  x[2] + nz + du[2] + dv[2]};

                        } else {

                            v0 = {x[0] + du[0],            x[1] + du[1],            x[2] + du[2]};
                            v1 = {x[0],                    x[1],                    x[2]};
                            v2 = {x[0] + du[0] + dv[0],
                                  x[1] + du[1] + dv[1],
                                  x[2] + du[2] + dv[2]};
                            v3 = {x[0] + dv[0],            x[1] + dv[1],            x[2] + dv[2]};
                        }

                        const bool transparent =
                            isTransparent(current.id);

                        auto& verts =
                            transparent
                                ? mesh.transparent.vertices
                                : mesh.solid.vertices;

                        auto& inds =
                            transparent
                                ? mesh.transparent.indices
                                : mesh.solid.indices;

                        uint32_t& offset =
                            transparent
                                ? blendOffset
                                : solidOffset;

                        verts.push_back(compress(
                            v0.x, v0.y, v0.z,
                            0, 0,
                            current.texture,
                            unpackAO(current.ao, 0)
                        ));

                        verts.push_back(compress(
                            v1.x, v1.y, v1.z,
                            w, 0,
                            current.texture,
                            unpackAO(current.ao, 1)
                        ));

                        verts.push_back(compress(
                            v2.x, v2.y, v2.z,
                            0, h,
                            current.texture,
                            unpackAO(current.ao, 2)
                        ));

                        verts.push_back(compress(
                            v3.x, v3.y, v3.z,
                            w, h,
                            current.texture,
                            unpackAO(current.ao, 3)
                        ));

                        const uint8_t a0 = unpackAO(current.ao, 0);
                        const uint8_t a1 = unpackAO(current.ao, 1);
                        const uint8_t a2 = unpackAO(current.ao, 2);
                        const uint8_t a3 = unpackAO(current.ao, 3);

                        if (a0 + a3 > a1 + a2) {

                            inds.push_back(offset + 0);
                            inds.push_back(offset + 1);
                            inds.push_back(offset + 3);

                            inds.push_back(offset + 0);
                            inds.push_back(offset + 3);
                            inds.push_back(offset + 2);

                        } else {

                            inds.push_back(offset + 0);
                            inds.push_back(offset + 1);
                            inds.push_back(offset + 2);

                            inds.push_back(offset + 1);
                            inds.push_back(offset + 3);
                            inds.push_back(offset + 2);
                        }

                        offset += 4;

                        for (int l = 0; l < h; ++l) {
                            for (int k = 0; k < w; ++k) {
                                mask[n + k + l * CHUNK_SIZE] = {};
                            }
                        }

                        i += w;
                        n += w;
                    }
                }
            }
        }

        return mesh;
    }

}