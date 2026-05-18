#include "cube/data/Mesh.hpp"

#include <cmath>

#include "cube/core/BlockID.hpp"
#include "cube/data/Chunk.hpp"
#include "cube/data/World.hpp"

namespace cube {

    enum Face{
        Top,
        Bottom,
        Left,
        Right,
        Near,
        Far
    };

    uint8_t getTexture(const BlockID block, const Face face) {
        if (block == GrassBlock) {
            if (face == Top) {
                return 1;
            }
            if (face == Bottom) {
                return 3;
            }
            return 2;
        }
        if (block == Dirt) {
            return 3;
        }
        if (block == Stone) {
            return 4;
        }
        if (block == Deepslate) {
            if (face == Top || face == Bottom) {
                return 6;
            }
            return 5;
        }
        if (block == Bedrock) {
            return 7;
        }
        return 0;
    }

    BlockID getVoxelAtGlobal(const glm::ivec3& chunkPos, const glm::ivec3& localPos, const World& world) {
        glm::ivec3 globalPos = chunkPos * 16 + localPos;

        glm::ivec3 targetChunkPos = glm::ivec3(
            static_cast<int>(std::floor(static_cast<float>(globalPos.x) / 16.0f)),
            static_cast<int>(std::floor(static_cast<float>(globalPos.y) / 16.0f)),
            static_cast<int>(std::floor(static_cast<float>(globalPos.z) / 16.0f))
        );

        glm::ivec3 targetLocalPos = globalPos - (targetChunkPos * 16);

        const std::shared_ptr<Chunk> targetChunk = world.getChunk(targetChunkPos);
        if (!targetChunk) {
            return BlockID::Air;
        }

        return static_cast<BlockID>(targetChunk->at(targetLocalPos) & 0xFF);
    }

    bool isTransparent(const BlockID id) {
        return id == BlockID::Water || id == BlockID::Lava;
    }

    uint8_t computeAO(const glm::ivec3& chunkPos, const glm::ivec3& p, const glm::ivec3& side1, const glm::ivec3& side2, const glm::ivec3& corner, const World& world) {
        bool s1 = getVoxelAtGlobal(chunkPos, p + side1, world) != BlockID::Air;
        bool s2 = getVoxelAtGlobal(chunkPos, p + side2, world) != BlockID::Air;
        bool c  = getVoxelAtGlobal(chunkPos, p + corner, world) != BlockID::Air;

        if (s1 && s2) {
            return 0;
        }
        return 3 - (s1 + s2 + c);
    }

    Mesh generateMesh(const glm::ivec3& pos, const World& world) {
        Mesh mesh;
        uint32_t solidOffset = 0;
        uint32_t blendOffset = 0;

        const glm::ivec3 normals[6] = {
            {0, 1, 0}, {0, -1, 0}, {-1, 0, 0}, {1, 0, 0}, {0, 0, 1}, {0, 0, -1}
        };

        const int axisU[6] = { 2, 2, 2, 2, 0, 0 };
        const int axisV[6] = { 0, 0, 1, 1, 1, 1 };
        const int axisD[6] = { 1, 1, 0, 0, 2, 2 };

        const glm::ivec3 aoSides[6][4][3] = {
            { {{0,0,-1}, {-1,0,0}, {-1,0,-1}}, {{0,0,-1}, {1,0,0}, {1,0,-1}}, {{0,0,1}, {-1,0,0}, {-1,0,1}}, {{0,0,1}, {1,0,0}, {1,0,1}} },
            { {{0,0,-1}, {-1,0,0}, {-1,0,-1}}, {{0,0,-1}, {1,0,0}, {1,0,-1}}, {{0,0,1}, {-1,0,0}, {-1,0,1}}, {{0,0,1}, {1,0,0}, {1,0,1}} },
            { {{0,0,-1}, {0,-1,0}, {0,-1,-1}}, {{0,0,-1}, {0,1,0}, {0,1,-1}}, {{0,0,1}, {0,-1,0}, {0,-1,1}}, {{0,0,1}, {0,1,0}, {0,1,1}} },
            { {{0,0,-1}, {0,-1,0}, {0,-1,-1}}, {{0,0,-1}, {0,1,0}, {0,1,-1}}, {{0,0,1}, {0,-1,0}, {0,-1,1}}, {{0,0,1}, {0,1,0}, {0,1,1}} },
            { {{-1,0,0}, {0,-1,0}, {-1,-1,0}}, {{-1,0,0}, {0,1,0}, {-1,1,0}}, {{1,0,0}, {0,-1,0}, {1,-1,0}}, {{1,0,0}, {0,1,0}, {1,1,0}} },
            { {{-1,0,0}, {0,-1,0}, {-1,-1,0}}, {{-1,0,0}, {0,1,0}, {-1,1,0}}, {{1,0,0}, {0,-1,0}, {1,-1,0}}, {{1,0,0}, {0,1,0}, {1,1,0}} }
        };

        struct FaceInfo {
            BlockID id = BlockID::Air;
            uint8_t textureLayer = 0;
            uint8_t ao[4] = {3, 3, 3, 3};
            bool operator==(const FaceInfo& o) const {
                return id == o.id && textureLayer == o.textureLayer &&
                       ao[0] == o.ao[0] && ao[1] == o.ao[1] && ao[2] == o.ao[2] && ao[3] == o.ao[3];
            }
        };

        for (int f = 0; f < 6; ++f) {
            Face faceType = static_cast<Face>(f);
            glm::ivec3 normal = normals[f];
            int u = axisU[f];
            int v = axisV[f];
            int d = axisD[f];

            glm::ivec3 x(0);
            std::vector<FaceInfo> mask(16 * 16);

            for (x[d] = 0; x[d] < 16; ++x[d]) {
                int n = 0;
                for (x[v] = 0; x[v] < 16; ++x[v]) {
                    for (x[u] = 0; x[u] < 16; ++x[u], ++n) {
                        BlockID current = getVoxelAtGlobal(pos, x, world);
                        BlockID neighbor = getVoxelAtGlobal(pos, x + normal, world);

                        if (current == BlockID::Air) {
                            continue;
                        }

                        bool visible = false;
                        if (neighbor == BlockID::Air) {
                            visible = true;
                        } else if (isTransparent(neighbor) && !isTransparent(current)) {
                            visible = true;
                        } else if (isTransparent(current) && isTransparent(neighbor) && current != neighbor) {
                            visible = true;
                        }

                        if (visible) {
                            mask[n].id = current;
                            mask[n].textureLayer = getTexture(current, faceType);

                            glm::ivec3 checkPos = x + normal;
                            mask[n].ao[0] = computeAO(pos, checkPos, aoSides[f][0][0], aoSides[f][0][1], aoSides[f][0][2], world);
                            mask[n].ao[1] = computeAO(pos, checkPos, aoSides[f][1][0], aoSides[f][1][1], aoSides[f][1][2], world);
                            mask[n].ao[2] = computeAO(pos, checkPos, aoSides[f][2][0], aoSides[f][2][1], aoSides[f][2][2], world);
                            mask[n].ao[3] = computeAO(pos, checkPos, aoSides[f][3][0], aoSides[f][3][1], aoSides[f][3][2], world);
                        }
                    }
                }

                n = 0;
                for (int j = 0; j < 16; ++j) {
                    for (int i = 0; i < 16; ) {
                        if (mask[n].id != BlockID::Air) {
                            int w, h;
                            FaceInfo currentFace = mask[n];

                            for (w = 1; i + w < 16 && mask[n + w] == currentFace; ++w){}

                            bool done = false;
                            for (h = 1; j + h < 16; ++h) {
                                for (int k = 0; k < w; ++k) {
                                    if (!(mask[n + k + h * 16] == currentFace)) {
                                        done = true;
                                        break;
                                    }
                                }
                                if (done) break;
                            }

                            x[u] = i; x[v] = j;
                            auto du = glm::ivec3{0}, dv = glm::ivec3{0};
                            du[u] = w; dv[v] = h;

                            glm::ivec3 v0 = x, v1 = x, v2 = x, v3 = x;
                            if (f == Top || f == Right || f == Near) {
                                v0 += normal; v1 += normal + du; v2 += normal + dv; v3 += normal + du + dv;
                            } else {
                                v0 += du; v1 += 0; v2 += du + dv; v3 += dv;
                            }

                            bool blendPass = isTransparent(currentFace.id);
                            std::vector<Vertex>& outVerts = blendPass ? mesh.transparent.vertices : mesh.solid.vertices;
                            std::vector<uint32_t>& outInds = blendPass ? mesh.transparent.indices : mesh.solid.indices;
                            uint32_t& activeOffset = blendPass ? blendOffset : solidOffset;

                            outVerts.push_back(compress(v0.x, v0.y, v0.z, 0, 0, currentFace.textureLayer, currentFace.ao[0]));
                            outVerts.push_back(compress(v1.x, v1.y, v1.z, w, 0, currentFace.textureLayer, currentFace.ao[1]));
                            outVerts.push_back(compress(v2.x, v2.y, v2.z, 0, h, currentFace.textureLayer, currentFace.ao[2]));
                            outVerts.push_back(compress(v3.x, v3.y, v3.z, w, h, currentFace.textureLayer, currentFace.ao[3]));

                            if (currentFace.ao[0] + currentFace.ao[3] > currentFace.ao[1] + currentFace.ao[2]) {
                                outInds.push_back(activeOffset + 0); outInds.push_back(activeOffset + 1); outInds.push_back(activeOffset + 3);
                                outInds.push_back(activeOffset + 0); outInds.push_back(activeOffset + 3); outInds.push_back(activeOffset + 2);
                            } else {
                                outInds.push_back(activeOffset + 0); outInds.push_back(activeOffset + 1); outInds.push_back(activeOffset + 2);
                                outInds.push_back(activeOffset + 1); outInds.push_back(activeOffset + 3); outInds.push_back(activeOffset + 2);
                            }

                            activeOffset += 4;

                            for (int l = 0; l < h; ++l) {
                                for (int k = 0; k < w; ++k) {
                                    mask[n + k + l * 16] = FaceInfo{};
                                }
                            }
                            i += w; n += w;
                        } else {
                            ++i; ++n;
                        }
                    }
                }
            }
        }

        return mesh;
    }

}
