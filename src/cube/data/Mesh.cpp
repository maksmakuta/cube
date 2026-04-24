#include <cube/data/Mesh.hpp>

namespace cube {

    bool ChunkNeighbors::isValid() const {
        return center != nullptr;
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

    const glm::vec2 faceUVs[4] = {
        {0.0f, 0.0f}, // Bottom-left
        {1.0f, 0.0f}, // Bottom-right
        {1.0f, 1.0f}, // Top-right
        {0.0f, 1.0f}  // Top-left
    };

    const glm::vec3 faceVertices[6][4] = {
        {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}}, // Front (+Z)
        {{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}}, // Back  (-Z)
        {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}}, // Top   (+Y)
        {{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}}, // Bottom(-Y)
        {{1,0,1}, {1,0,0}, {1,1,0}, {1,1,1}}, // Right (+X)
        {{0,0,0}, {0,0,1}, {0,1,1}, {0,1,0}}  // Left  (-X)
    };

    RenderableMesh getMesh(const ChunkNeighbors& n, const glm::ivec3& chunkPos) {
        RenderableMesh mesh;
        mesh.pos = chunkPos;

        uint32_t vertexCount = 0;

        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                for (int z = 0; z < 16; ++z) {

                    Block current = n.center->view()[x, y, z];
                    if (current == Air) continue;

                    BlockData data = getBlockData(current);

                    for (int f = 0; f < 6; ++f) {
                        const glm::ivec3 dirs[6] = {
                            {0,0,1}, {0,0,-1}, {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}
                        };

                        if (const Block neighbor = n.getBlock({x + dirs[f].x, y + dirs[f].y, z + dirs[f].z}); neighbor == Air) {
                            const float texID = (dirs[f].y > 0) ? data.top : (dirs[f].y < 0) ? data.bottom : data.side;

                            for (int v = 0; v < 4; ++v) {
                                glm::vec2 uv = faceUVs[v];
                                mesh.vertices.push_back({
                                    glm::vec3(x, y, z) + faceVertices[f][v],
                                    glm::vec3(dirs[f]),
                                    glm::vec3(uv, texID)
                                });
                            }

                            mesh.indices.insert(mesh.indices.end(), {
                                vertexCount + 0, vertexCount + 1, vertexCount + 2,
                                vertexCount + 0, vertexCount + 2, vertexCount + 3
                            });

                            vertexCount += 4;
                        }
                    }
                }
            }
        }
        return mesh;
    }

}