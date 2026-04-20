#include <cube/task/Meshing.hpp>

#include "cube/core/Block.hpp"

namespace cube {

    struct ChunkNeighborhood {
        uint8_t blocks[18 * 18 * 18];

        uint8_t get(const int x, const int y, const int z) const {
            return blocks[x + y * 18 + z * 18 * 18];
        }
    };

    uint8_t get_ao(uint8_t side1, uint8_t side2, uint8_t corner) {
        if (side1 && side2) return 0;
        return 3 - (side1 + side2 + corner);
    }

    uint8_t calculate_face_ao(const ChunkNeighborhood& nb, glm::ivec3 p, int d) {
        // This logic changes based on the 6 directions
        // For +Y (d=3): we look at blocks at p.y + 1
        // Neighbors: (x-1, z), (x+1, z), (x, z-1), (x, z+1) and the 4 corners
        // Result: 4 corners * 2 bits each = 8 bits

        // Implementation for +Y:
        uint8_t s1 = nb.get(p.x,     p.y + 1, p.z + 1) != 0;
        uint8_t s2 = nb.get(p.x,     p.y + 1, p.z - 1) != 0;
        uint8_t s3 = nb.get(p.x + 1, p.y + 1, p.z)     != 0;
        uint8_t s4 = nb.get(p.x - 1, p.y + 1, p.z)     != 0;

        uint8_t c1 = nb.get(p.x + 1, p.y + 1, p.z + 1) != 0;
        uint8_t c2 = nb.get(p.x + 1, p.y + 1, p.z - 1) != 0;
        uint8_t c3 = nb.get(p.x - 1, p.y + 1, p.z - 1) != 0;
        uint8_t c4 = nb.get(p.x - 1, p.y + 1, p.z + 1) != 0;

        uint8_t ao0 = get_ao(s4, s1, c4);
        uint8_t ao1 = get_ao(s3, s1, c1);
        uint8_t ao2 = get_ao(s3, s2, c2);
        uint8_t ao3 = get_ao(s4, s2, c3);

        return (ao0) | (ao1 << 2) | (ao2 << 4) | (ao3 << 6);
    }


    void add_quad(MeshResult& result, int ux, int vy, int slice, int w, int h, int d, uint16_t currProf) {
    uint8_t blockId = static_cast<uint8_t>(currProf >> 8);

    // Extract 4 AO corners (2 bits each)
    uint8_t ao0 = (currProf      ) & 0x03;
    uint8_t ao1 = (currProf >> 2 ) & 0x03;
    uint8_t ao2 = (currProf >> 4 ) & 0x03;
    uint8_t ao3 = (currProf >> 6 ) & 0x03;

    // Define the 4 corners of the quad in 3D
    glm::ivec3 v0, v1, v2, v3;
    int axis = d / 2;
    int u = (axis + 1) % 3;
    int v = (axis + 2) % 3;

    // Offset for the face position
    // If it's a "Forward" face (+X, +Y, +Z), we shift the slice by 1
    int offset = (d % 2 != 0) ? 1 : 0;

    auto set_pos = [&](glm::ivec3& vert, int u_val, int v_val) {
        vert[axis] = slice + offset;
        vert[u] = u_val;
        vert[v] = v_val;
    };

    set_pos(v0, ux,     vy);
    set_pos(v1, ux + w, vy);
    set_pos(v2, ux + w, vy + h);
    set_pos(v3, ux,     vy + h);

    // Get the base vertex index for this quad
    uint32_t baseIdx = static_cast<uint32_t>(result.vertices.size());

    // Pack vertices (x, y, z, ao, u, v, normal, light, sway, id)
    // We'll set light to 15 (max) for now and sway to false
    bool sway = (blockId == static_cast<uint8_t>(Kelp) || blockId == static_cast<uint8_t>(TallGrass));

    result.vertices.push_back(Vertex::pack(v0.x, v0.y, v0.z, ao0, 0, 0, d, 15, sway, blockId));
    result.vertices.push_back(Vertex::pack(v1.x, v1.y, v1.z, ao1, 1, 0, d, 15, sway, blockId));
    result.vertices.push_back(Vertex::pack(v2.x, v2.y, v2.z, ao2, 1, 1, d, 15, sway, blockId));
    result.vertices.push_back(Vertex::pack(v3.x, v3.y, v3.z, ao3, 0, 1, d, 15, sway, blockId));

    // --- Triangulation / Anisotropic Fix ---
    // Flip the quad diagonal if AO suggests a better interpolation path
    if (ao0 + ao2 > ao1 + ao3) {
        // Triangle 1: 0-1-2, Triangle 2: 0-2-3
        result.indices.push_back(baseIdx + 0);
        result.indices.push_back(baseIdx + 1);
        result.indices.push_back(baseIdx + 2);
        result.indices.push_back(baseIdx + 0);
        result.indices.push_back(baseIdx + 2);
        result.indices.push_back(baseIdx + 3);
    } else {
        // Triangle 1: 1-2-3, Triangle 2: 1-3-0
        result.indices.push_back(baseIdx + 1);
        result.indices.push_back(baseIdx + 2);
        result.indices.push_back(baseIdx + 3);
        result.indices.push_back(baseIdx + 1);
        result.indices.push_back(baseIdx + 3);
        result.indices.push_back(baseIdx + 0);
    }
}

    MeshResult mesh(World& world, MeshTask task) {
    MeshResult result;
    result.pos = task.pos;

    ChunkNeighborhood nb;
        {
        if (!world.is_ready_to_mesh(task.pos)) return result;

        for (int z = -1; z <= 16; ++z) {
            for (int y = -1; y <= 16; ++y) {
                for (int x = -1; x <= 16; ++x) {
                    nb.blocks[(x + 1) + (y + 1) * 18 + (z + 1) * 324] =
                        world.get_block(task.pos * CHUNK_SIZE + glm::ivec3(x, y, z));
                }
            }
        }
        }

    // 2. Greedy Loop over 6 directions
    // 0: -X, 1: +X, 2: -Y, 3: +Y, 4: -Z, 5: +Z
    for (int d = 0; d < 6; ++d) {
        int axis = d / 2;
        bool isBack = (d % 2 == 0);

        int u = (axis + 1) % 3;
        int v = (axis + 2) % 3;

        // Per-slice mask and profile buffer
        std::array<bool, 16 * 16> mask{};
        std::array<uint16_t, 16 * 16> profiles{};

        // Offset for the neighbor check based on direction
        glm::ivec3 dirVec(0);
        dirVec[axis] = isBack ? -1 : 1;

        for (int slice = 0; slice < 16; ++slice) {
            mask.fill(false);
            bool sliceEmpty = true;

            // Step A: Generate Profiles for this slice
            for (int vy = 0; vy < 16; ++vy) {
                for (int ux = 0; ux < 16; ++ux) {
                    glm::ivec3 local(0);
                    local[axis] = slice;
                    local[u] = ux;
                    local[v] = vy;

                    // Neighbors are +1 because nb is 18x18x18
                    uint8_t current = nb.get(local.x + 1, local.y + 1, local.z + 1);
                    if (current == 0) continue;

                    // Face Culling: Is the neighbor in 'd' direction transparent?
                    uint8_t neighbor = nb.get(local.x + 1 + dirVec.x, local.y + 1 + dirVec.y, local.z + 1 + dirVec.z);
                    if (neighbor != 0) continue; // Face hidden

                    // AO Calculation (Requires offsets relative to face normal)
                    // Simplified: get_face_ao returns 4 corners packed into 8 bits
                    uint8_t ao = calculate_face_ao(nb, local + glm::ivec3(1), d);

                    profiles[ux + vy * 16] = (uint16_t(current) << 8) | ao;
                    sliceEmpty = false;
                }
            }

            if (sliceEmpty) continue;

            // Step B: Greedy Sweep
            for (int vy = 0; vy < 16; ++vy) {
                for (int ux = 0; ux < 16; ) {
                    uint16_t currProf = profiles[ux + vy * 16];
                    if (currProf == 0 || mask[ux + vy * 16]) { ux++; continue; }

                    int w = 1;
                    while (ux + w < 16 && !mask[(ux + w) + vy * 16] && profiles[(ux + w) + vy * 16] == currProf) w++;

                    int h = 1;
                    while (vy + h < 16) {
                        bool rowMatch = true;
                        for (int k = 0; k < w; ++k) {
                            if (mask[(ux + k) + (vy + h) * 16] || profiles[(ux + k) + (vy + h) * 16] != currProf) {
                                rowMatch = false; break;
                            }
                        }
                        if (rowMatch) h++; else break;
                    }

                    // Step C: Push Quad
                    add_quad(result, ux, vy, slice, w, h, d, currProf);

                    for (int i = 0; i < h; ++i)
                        for (int j = 0; j < w; ++j)
                            mask[(ux + j) + (vy + i) * 16] = true;

                    ux += w;
                }
            }
        }
    }
    return result;
}

}
