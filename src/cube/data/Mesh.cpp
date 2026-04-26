#include <cube/data/Mesh.hpp>
#include <glm/vec2.hpp>

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

   const glm::vec2 faceUVs[4] = {
        {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}
    };

    const glm::vec3 faceVertices[6][4] = {
        {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}}, // Front (+Z)
        {{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}}, // Back  (-Z)
        {{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}}, // Top   (+Y)
        {{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}}, // Bottom(-Y)
        {{1,0,1}, {1,0,0}, {1,1,0}, {1,1,1}}, // Right (+X)
        {{0,0,0}, {0,0,1}, {0,1,1}, {0,1,0}}  // Left  (-X)
    };

    // HOISTED: Moved out of the loop so it isn't recreated 24,000 times per chunk
    const glm::ivec3 dirs[6] = {
        {0,0,1}, {0,0,-1}, {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}
    };

    RenderableMesh getMesh(const ChunkNeighbors& n, const glm::ivec3& chunkPos) {
        RenderableMesh mesh;
        mesh.pos = chunkPos;

        mesh.vertices.reserve(4096);
        mesh.indices.reserve(6144);

        uint32_t vertexCount = 0;

        const auto centerView = n.center->view();

        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                for (int z = 0; z < 16; ++z) {

                    const Block current = centerView[x, y, z];
                    if (current == Air) continue;

                    BlockData data = getBlockData(current);

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

                        if (neighbor == Air) {
                            const float texID = (dirs[f].y > 0) ? data.top : (dirs[f].y < 0) ? data.bottom : data.side;

                            for (int v = 0; v < 4; ++v) {
                                mesh.vertices.push_back({
                                    glm::vec3(x, y, z) + faceVertices[f][v],
                                    glm::vec3(dirs[f]),
                                    glm::vec3(faceUVs[v], texID)
                                });
                            }

                            mesh.indices.push_back(vertexCount + 0);
                            mesh.indices.push_back(vertexCount + 1);
                            mesh.indices.push_back(vertexCount + 2);
                            mesh.indices.push_back(vertexCount + 0);
                            mesh.indices.push_back(vertexCount + 2);
                            mesh.indices.push_back(vertexCount + 3);

                            vertexCount += 4;
                        }
                    }
                }
            }
        }

        mesh.vertices.shrink_to_fit();
        mesh.indices.shrink_to_fit();

        return mesh;
    }

}
