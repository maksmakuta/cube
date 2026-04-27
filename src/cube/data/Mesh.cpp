#include <cube/data/Mesh.hpp>

#include <glm/vec2.hpp>
#include <glm/common.hpp>

namespace cube {

    bool ChunkNeighbors::isValid() const {
        return center && px && nx && py && ny && pz && nz;
    }

    Block ChunkNeighbors::getBlock(const glm::ivec3& pos) const {
        if (pos.x >= 0 && pos.x < 16 && pos.y >= 0 && pos.y < 16 && pos.z >= 0 && pos.z < 16) {
            return center->view()[pos.x, pos.y, pos.z];
        }
        const int lx = pos.x & 15;
        const int ly = pos.y & 15;
        const int lz = pos.z & 15;
        if (pos.x >= 16) return px ? px->view()[lx, ly, lz] : Air;
        if (pos.x < 0)   return nx ? nx->view()[lx, ly, lz] : Air;
        if (pos.y >= 16) return py ? py->view()[lx, ly, lz] : Air;
        if (pos.y < 0)   return ny ? ny->view()[lx, ly, lz] : Air;
        if (pos.z >= 16) return pz ? pz->view()[lx, ly, lz] : Air;
        if (pos.z < 0)   return nz ? nz->view()[lx, ly, lz] : Air;
        return Air;
    }

    constexpr glm::vec2 faceUVs[4] = {
        {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}
    };

    const glm::vec3 faceVertices[6][4] = {
        {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}},
        {{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}},
        {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}},
        {{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}},
        {{1,0,1}, {1,0,0}, {1,1,0}, {1,1,1}},
        {{0,0,0}, {0,0,1}, {0,1,1}, {0,1,0}}
    };

    constexpr glm::vec3 crossVertices[4][4] = {
        {{0,0,0}, {1,0,1}, {1,1,1}, {0,1,0}},
        {{1,0,1}, {0,0,0}, {0,1,0}, {1,1,1}},
        {{1,0,0}, {0,0,1}, {0,1,1}, {1,1,0}},
        {{0,0,1}, {1,0,0}, {1,1,0}, {0,1,1}}
    };

    const glm::ivec3 dirs[6] = {
        {0,0,1}, {0,0,-1}, {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}
    };

    RenderableMesh getMesh(const ChunkNeighbors& n, const glm::ivec3& chunkPos) {
        RenderableMesh mesh;
        mesh.pos = chunkPos;
        mesh.solidVertices.reserve(3000);
        mesh.solidIndices.reserve(4500);
        mesh.transVertices.reserve(1000);
        mesh.transIndices.reserve(1500);

        uint32_t solidVertexCount = 0;
        uint32_t transVertexCount = 0;

        const auto centerView = n.center->view();

        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                for (int z = 0; z < 16; ++z) {

                    const Block current = centerView[x, y, z];
                    if (current == Air) continue;

                    auto [top, side, bottom, count, sideOverlay, isTinted, shape] = getBlockData(current);

                    bool isTranslucent = shape == BlockShape::Liquid;

                    auto& currentVertices = isTranslucent ? mesh.transVertices : mesh.solidVertices;
                    auto& currentIndices = isTranslucent ? mesh.transIndices : mesh.solidIndices;
                    uint32_t& currentVertexCount = isTranslucent ? transVertexCount : solidVertexCount;


                    glm::vec4 tintColor(1.0f);
                    // if (isTinted) {
                    //     const int index2D = x + z * 16;
                    //     const float temp = n.center->biomeData[index2D].temp;
                    //     const float moist = n.center->biomeData[index2D].moisture;
                    //
                    //     constexpr glm::vec4 colorHotWet(0.1f, 0.8f, 0.1f,1.f);
                    //     constexpr glm::vec4 colorHotDry(0.7f, 0.8f, 0.2f,1.f);
                    //     constexpr glm::vec4 colorColdWet(0.2f, 0.5f, 0.3f,1.f);
                    //     constexpr glm::vec4 colorColdDry(0.6f, 0.6f, 0.6f,1.f);
                    //
                    //     float t = glm::clamp((temp + 1.0f) * 0.5f, 0.0f, 1.0f);
                    //     float m = glm::clamp((moist + 1.0f) * 0.5f, 0.0f, 1.0f);
                    //
                    //     glm::vec4 mixHot = glm::mix(colorHotDry, colorHotWet, m);
                    //     glm::vec4 mixCold = glm::mix(colorColdDry, colorColdWet, m);
                    //
                    //     tintColor = glm::mix(mixCold, mixHot, t);
                    // }else
                    if (shape == BlockShape::Liquid) {
                        tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.65f);
                    }

                    if (shape == BlockShape::Cross) {
                        for (const auto & crossVertice : crossVertices) {
                            for (int v = 0; v < 4; ++v) {
                                currentVertices.push_back({
                                    glm::vec3(x, y, z) + crossVertice[v],
                                    glm::vec3(0.0f, 1.0f, 0.0f),
                                    glm::vec4(faceUVs[v], side, -1.0f),
                                    tintColor,
                                    static_cast<float>(count)
                                });
                            }
                            currentIndices.push_back(currentVertexCount + 0);
                            currentIndices.push_back(currentVertexCount + 1);
                            currentIndices.push_back(currentVertexCount + 2);
                            currentIndices.push_back(currentVertexCount + 0);
                            currentIndices.push_back(currentVertexCount + 2);
                            currentIndices.push_back(currentVertexCount + 3);
                            currentVertexCount += 4;
                        }
                        continue;
                    }

                    if (shape == BlockShape::Panel) {
                        for (constexpr int horizontalFaces[4] = {0, 1, 4, 5}; int f : horizontalFaces) {
                            int nx = x + dirs[f].x;
                            int ny = y + dirs[f].y;
                            int nz = z + dirs[f].z;

                            Block neighbor = Air;
                            if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16 && nz >= 0 && nz < 16) {
                                neighbor = centerView[nx, ny, nz];
                            } else {
                                neighbor = n.getBlock({nx, ny, nz});
                            }

                            if (neighbor != Air && getBlockData(neighbor).shape == BlockShape::Cube) {
                                int oppositeFace = f % 2 == 0 ? f + 1 : f - 1;
                                glm::vec3 pushInward = -glm::vec3(dirs[f]) * 0.01f;

                                for (int v = 0; v < 4; ++v) {
                                    glm::vec3 baseVert = faceVertices[oppositeFace][v];
                                    glm::vec3 shiftedVert = baseVert + glm::vec3(dirs[f]);

                                    currentVertices.push_back({
                                        glm::vec3(x, y, z) + shiftedVert + pushInward,
                                        -glm::vec3(dirs[f]),
                                        glm::vec4(faceUVs[v], side, -1.0f),
                                        tintColor,
                                        static_cast<float>(count)
                                    });
                                }
                                currentIndices.push_back(currentVertexCount + 0);
                                currentIndices.push_back(currentVertexCount + 1);
                                currentIndices.push_back(currentVertexCount + 2);
                                currentIndices.push_back(currentVertexCount + 0);
                                currentIndices.push_back(currentVertexCount + 2);
                                currentIndices.push_back(currentVertexCount + 3);
                                currentVertexCount += 4;
                            }
                        }
                        continue;
                    }

                    for (int f = 0; f < 6; ++f) {
                        int nx = x + dirs[f].x;
                        int ny = y + dirs[f].y;
                        int nz = z + dirs[f].z;

                        Block neighbor = Air;

                        if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16 && nz >= 0 && nz < 16) {
                            neighbor = centerView[nx, ny, nz];
                        } else {
                            neighbor = n.getBlock({nx, ny, nz});
                        }

                        bool shouldDrawFace = false;

                        if (neighbor == Air || neighbor == Cactus) {
                            shouldDrawFace = true;
                        } else {
                            BlockData neighborData = getBlockData(neighbor);

                            if (shape == BlockShape::Liquid) {
                                if (dirs[f].y > 0 && neighbor != current && neighborData.shape != BlockShape::Cube) {
                                    shouldDrawFace = true;
                                } else {
                                    shouldDrawFace = false;
                                }
                            }
                            else {
                                shouldDrawFace = neighborData.shape != BlockShape::Cube;
                            }
                        }

                        if (shouldDrawFace) {
                            const float texID = dirs[f].y > 0 ? top : dirs[f].y < 0 ? bottom : side;

                            float overlayID = -1.0f;
                            if (sideOverlay != -1 && dirs[f].y == 0) {
                                overlayID = static_cast<float>(sideOverlay);
                            }

                            for (int v = 0; v < 4; ++v) {
                                glm::vec3 vertPos = faceVertices[f][v];
                                if (shape == BlockShape::Liquid && f == 2) {
                                    vertPos.y -= 0.1f;
                                }

                                if (shape == BlockShape::Cube && current == Cactus) {
                                    constexpr float INSET = 1.0f / 16.0f;
                                    if (f != 2)
                                        vertPos -= glm::vec3(dirs[f]) * INSET;
                                }

                                currentVertices.push_back({
                                    glm::vec3(x, y, z) + vertPos,
                                    glm::vec3(dirs[f]),
                                    glm::vec4(faceUVs[v], texID, overlayID),
                                    tintColor,
                                    static_cast<float>(count)
                                });
                            }

                            currentIndices.push_back(currentVertexCount + 0);
                            currentIndices.push_back(currentVertexCount + 1);
                            currentIndices.push_back(currentVertexCount + 2);
                            currentIndices.push_back(currentVertexCount + 0);
                            currentIndices.push_back(currentVertexCount + 2);
                            currentIndices.push_back(currentVertexCount + 3);

                            currentVertexCount += 4;
                        }
                    }
                }
            }
        }

        mesh.solidVertices.shrink_to_fit();
        mesh.solidIndices.shrink_to_fit();
        mesh.transVertices.shrink_to_fit();
        mesh.transIndices.shrink_to_fit();

        return mesh;
    }
}
