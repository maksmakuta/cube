#include "cube/data/Mesh.hpp"

#include <memory>

#include "cube/core/BlockID.hpp"
#include "cube/data/Chunk.hpp"
#include "cube/data/World.hpp"

namespace cube {

    using ChunkData = std::array<std::shared_ptr<Chunk>, 27>;

    [[nodiscard]] inline int chunkIndex(const int dx, const int dy, const int dz) noexcept {
        return (dz + 1) * 9 + (dy + 1) * 3 + (dx + 1);
    }

    ChunkData getChunkData(const glm::ivec3& pos, const World& world) {
        ChunkData data{};

        for (int dz = -1; dz <= 1; ++dz) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    const int index = chunkIndex(dx, dy, dz);

                    data[index] = world.getChunk(
                        pos + glm::ivec3(dx, dy, dz)
                    );
                }
            }
        }

        return data;
    }
Mesh generateMesh(const glm::ivec3& pos, const World& world) {
        Mesh mesh;

        ChunkData chunks = getChunkData(pos, world);

        constexpr glm::ivec3 normals[6] = {
            { 0,  1,  0}, // top
            { 0, -1,  0}, // bottom
            {-1,  0,  0}, // left
            { 1,  0,  0}, // right
            { 0,  0, -1}, // front
            { 0,  0,  1}  // back
        };

        struct MaskCell {
            BlockID block = BlockID::Air;

            uint8_t texture = 0;

            uint8_t ao[4]{};

            uint16_t tint = 0;

            bool transparent = false;
            bool flip = false;
            bool valid = false;
        };

        auto getBlock = [&](int x, int y, int z) -> BlockID {
            int cx = 0;
            int cy = 0;
            int cz = 0;

            if (x < 0) {
                cx = -1;
                x += CHUNK_SIZE;
            }
            else if (x >= CHUNK_SIZE) {
                cx = 1;
                x -= CHUNK_SIZE;
            }

            if (y < 0) {
                cy = -1;
                y += CHUNK_SIZE;
            }
            else if (y >= CHUNK_SIZE) {
                cy = 1;
                y -= CHUNK_SIZE;
            }

            if (z < 0) {
                cz = -1;
                z += CHUNK_SIZE;
            }
            else if (z >= CHUNK_SIZE) {
                cz = 1;
                z -= CHUNK_SIZE;
            }

            const auto& chunk = chunks[chunkIndex(cx, cy, cz)];

            if (!chunk)
                return BlockID::Air;

            return static_cast<BlockID>(chunk->at({x, y, z}));
        };

        auto solid = [&](BlockID id) {
            return id != BlockID::Air &&
                   !getBlockInfo(id).transparent;
        };

        auto visible = [&](BlockID a, BlockID b) {
            if (b == BlockID::Air)
                return true;

            const auto& infoA = getBlockInfo(a);
            const auto& infoB = getBlockInfo(b);

            if (infoB.transparent && a != b)
                return true;

            return false;
        };

        auto vertexAO = [](bool s1, bool s2, bool c) -> uint8_t {
            if (s1 && s2)
                return 3;

            return s1 + s2 + c;
        };

        auto calcAO = [&](int face, int x, int y, int z, uint8_t out[4]) {

            auto occ = [&](int ox, int oy, int oz) {
                return solid(getBlock(x + ox, y + oy, z + oz));
            };

            switch (face) {

                // TOP
                case 0: {
                    out[0] = vertexAO(
                        occ(-1, 1, 0),
                        occ(0, 1,-1),
                        occ(-1,1,-1)
                    );

                    out[1] = vertexAO(
                        occ(1,1,0),
                        occ(0,1,-1),
                        occ(1,1,-1)
                    );

                    out[2] = vertexAO(
                        occ(1,1,0),
                        occ(0,1,1),
                        occ(1,1,1)
                    );

                    out[3] = vertexAO(
                        occ(-1,1,0),
                        occ(0,1,1),
                        occ(-1,1,1)
                    );
                    break;
                }

                    // BOTTOM
                case 1: {
                    out[0] = vertexAO(
                        occ(-1,-1,0),
                        occ(0,-1,-1),
                        occ(-1,-1,-1)
                    );

                    out[1] = vertexAO(
                        occ(1,-1,0),
                        occ(0,-1,-1),
                        occ(1,-1,-1)
                    );

                    out[2] = vertexAO(
                        occ(1,-1,0),
                        occ(0,-1,1),
                        occ(1,-1,1)
                    );

                    out[3] = vertexAO(
                        occ(-1,-1,0),
                        occ(0,-1,1),
                        occ(-1,-1,1)
                    );
                    break;
                }

                default:
                    out[0] = out[1] = out[2] = out[3] = 0;
                    break;
            }
        };

        auto sameMask = [](const MaskCell& a, const MaskCell& b) {
            if (!a.valid || !b.valid)
                return false;

            if (a.block != b.block)
                return false;

            if (a.texture != b.texture)
                return false;

            if (a.tint != b.tint)
                return false;

            if (a.flip != b.flip)
                return false;

            for (int i = 0; i < 4; ++i) {
                if (a.ao[i] != b.ao[i])
                    return false;
            }

            return true;
        };

        auto emitQuad = [&](MeshData& dst,
                            int face,
                            glm::ivec3 p,
                            int w,
                            int h,
                            const MaskCell& m)
        {
            const uint32_t index =
                static_cast<uint32_t>(dst.vertices.size());

            auto push = [&](int x, int y, int z,
                            int u, int v,
                            uint8_t ao)
            {
                dst.vertices.push_back(
                    compress(
                        x, y, z,
                        u, v,
                        m.texture,
                        ao,
                        m.tint
                    )
                );
            };

            switch (face) {

                // TOP
                case 0:
                    push(p.x,     p.y+1, p.z,     0, 0, m.ao[0]);
                    push(p.x+w,   p.y+1, p.z,     w, 0, m.ao[1]);
                    push(p.x+w,   p.y+1, p.z+h,   w, h, m.ao[2]);
                    push(p.x,     p.y+1, p.z+h,   0, h, m.ao[3]);
                    break;

                    // BOTTOM
                case 1:
                    push(p.x,     p.y, p.z,     0, 0, m.ao[0]);
                    push(p.x+w,   p.y, p.z,     w, 0, m.ao[1]);
                    push(p.x+w,   p.y, p.z+h,   w, h, m.ao[2]);
                    push(p.x,     p.y, p.z+h,   0, h, m.ao[3]);
                    break;

                default:
                    return;
            }

            if (!m.flip) {
                dst.indices.insert(dst.indices.end(), {
                    index + 0,
                    index + 1,
                    index + 2,

                    index + 0,
                    index + 2,
                    index + 3
                });
            }
            else {
                dst.indices.insert(dst.indices.end(), {
                    index + 0,
                    index + 1,
                    index + 3,

                    index + 1,
                    index + 2,
                    index + 3
                });
            }
        };

        std::array<MaskCell, CHUNK_SIZE * CHUNK_SIZE> mask;

        for (int face = 0; face < 2; ++face) {

            for (int layer = 0; layer < CHUNK_SIZE; ++layer) {

                // BUILD MASK

                for (int z = 0; z < CHUNK_SIZE; ++z) {
                    for (int x = 0; x < CHUNK_SIZE; ++x) {

                        const int idx = z * CHUNK_SIZE + x;

                        int bx = x;
                        int by = layer;
                        int bz = z;

                        if (face == 1)
                            by = layer;

                        const BlockID self =
                            getBlock(bx, by, bz);

                        if (self == BlockID::Air) {
                            mask[idx].valid = false;
                            continue;
                        }

                        const auto& info =
                            getBlockInfo(self);

                        if (info.type != MeshType::Cube) {
                            mask[idx].valid = false;
                            continue;
                        }

                        const glm::ivec3 n =
                            normals[face];

                        const BlockID neighbor =
                            getBlock(
                                bx + n.x,
                                by + n.y,
                                bz + n.z
                            );

                        if (!visible(self, neighbor)) {
                            mask[idx].valid = false;
                            continue;
                        }

                        auto& cell = mask[idx];

                        cell.valid = true;
                        cell.block = self;
                        cell.transparent = info.transparent;

                        switch (face) {
                            case 0:
                                cell.texture = info.top;
                                break;

                            case 1:
                                cell.texture = info.bottom;
                                break;

                            default:
                                cell.texture = info.side;
                                break;
                        }

                        cell.tint = packRGB565(255,255,255);

                        calcAO(face, bx, by, bz, cell.ao);

                        cell.flip =
                            (cell.ao[0] + cell.ao[2]) >
                            (cell.ao[1] + cell.ao[3]);
                    }
                }

                // GREEDY MERGE

                for (int z = 0; z < CHUNK_SIZE; ++z) {
                    for (int x = 0; x < CHUNK_SIZE;) {

                        const int idx =
                            z * CHUNK_SIZE + x;

                        auto& cell = mask[idx];

                        if (!cell.valid) {
                            ++x;
                            continue;
                        }

                        int width = 1;

                        while (x + width < CHUNK_SIZE) {

                            const auto& next =
                                mask[z * CHUNK_SIZE + x + width];

                            if (!sameMask(cell, next))
                                break;

                            ++width;
                        }

                        int height = 1;
                        bool done = false;

                        while (z + height < CHUNK_SIZE && !done) {

                            for (int k = 0; k < width; ++k) {

                                const auto& next =
                                    mask[(z + height) * CHUNK_SIZE + x + k];

                                if (!sameMask(cell, next)) {
                                    done = true;
                                    break;
                                }
                            }

                            if (!done)
                                ++height;
                        }

                        MeshData& dst =
                            cell.transparent
                            ? mesh.transparent
                            : mesh.solid;

                        emitQuad(
                            dst,
                            face,
                            {x, layer, z},
                            width,
                            height,
                            cell
                        );

                        for (int dz = 0; dz < height; ++dz) {
                            for (int dx = 0; dx < width; ++dx) {
                                mask[(z + dz) * CHUNK_SIZE + x + dx].valid = false;
                            }
                        }

                        x += width;
                    }
                }
            }
        }

        return mesh;
    }

}