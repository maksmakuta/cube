#include "ChunkMesh.hpp"

namespace cube {

    void ChunkMesh::reserve(const size_t v, const size_t i) {
        vertices.reserve(v);
        indices.reserve(i);
    }

    void ChunkMesh::clear() {
        vertices.clear();
        indices.clear();
    }

    constexpr glm::vec3 DIRECTIONS[6] = {
        { 1, 0, 0},
         {-1, 0, 0},
         { 0, 1, 0},
         { 0,-1, 0},
         { 0, 0, 1},
         { 0, 0,-1},
    };

    constexpr glm::vec2 TEXTURE_UV[4] = {
        {0,0},
        {0,1},
        {1,1},
        {1,0},
    };

    constexpr int INDICES[6] = {
        0, 1, 2,
        2, 3, 0
    };

    const glm::vec3 FACES[6][4] = {
        {
            {1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
        },
        {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 1.0f},
        },
        {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f},
        },
        {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 1.0f},
        },
        {
            {0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
        },
        {
            {1.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
        },
    };

    constexpr glm::ivec3 AO_OFFSETS[6][4][3] = {
       // Right face (+X) - faces in +X direction
    {
        {{1, 1, 0}, {1, 0, 1}, {1, 1, 1}},   // vertex 0: top-front
        {{1, -1, 0}, {1, 0, 1}, {1, -1, 1}}, // vertex 1: bottom-front
        {{1, -1, 0}, {1, 0, -1}, {1, -1, -1}}, // vertex 2: bottom-back
        {{1, 1, 0}, {1, 0, -1}, {1, 1, -1}}, // vertex 3: top-back
    },
    // Left face (-X) - faces in -X direction
    {
        {{-1, 1, 0}, {-1, 0, -1}, {-1, 1, -1}}, // vertex 0: top-back
        {{-1, -1, 0}, {-1, 0, -1}, {-1, -1, -1}}, // vertex 1: bottom-back
        {{-1, -1, 0}, {-1, 0, 1}, {-1, -1, 1}}, // vertex 2: bottom-front
        {{-1, 1, 0}, {-1, 0, 1}, {-1, 1, 1}},   // vertex 3: top-front
    },
    // Top face (+Y) - faces in +Y direction
    {
        {{-1, 1, 0}, {0, 1, -1}, {-1, 1, -1}}, // vertex 0: left-back
        {{-1, 1, 0}, {0, 1, 1}, {-1, 1, 1}},   // vertex 1: left-front
        {{1, 1, 0}, {0, 1, 1}, {1, 1, 1}},     // vertex 2: right-front
        {{1, 1, 0}, {0, 1, -1}, {1, 1, -1}},   // vertex 3: right-back
    },
    // Bottom face (-Y) - faces in -Y direction
    {
        {{-1, -1, 0}, {0, -1, 1}, {-1, -1, 1}},   // vertex 0: left-front
        {{-1, -1, 0}, {0, -1, -1}, {-1, -1, -1}}, // vertex 1: left-back
        {{1, -1, 0}, {0, -1, -1}, {1, -1, -1}},   // vertex 2: right-back
        {{1, -1, 0}, {0, -1, 1}, {1, -1, 1}},     // vertex 3: right-front
    },
    // Front face (+Z) - faces in +Z direction
    {
        {{-1, 0, 1}, {0, 1, 1}, {-1, 1, 1}},  // vertex 0: left-top
        {{-1, 0, 1}, {0, -1, 1}, {-1, -1, 1}}, // vertex 1: left-bottom
        {{1, 0, 1}, {0, -1, 1}, {1, -1, 1}},   // vertex 2: right-bottom
        {{1, 0, 1}, {0, 1, 1}, {1, 1, 1}},     // vertex 3: right-top
    },
    // Back face (-Z) - faces in -Z direction
    {
        {{1, 0, -1}, {0, 1, -1}, {1, 1, -1}},     // vertex 0: right-top
        {{1, 0, -1}, {0, -1, -1}, {1, -1, -1}},   // vertex 1: right-bottom
        {{-1, 0, -1}, {0, -1, -1}, {-1, -1, -1}}, // vertex 2: left-bottom
        {{-1, 0, -1}, {0, 1, -1}, {-1, 1, -1}},   // vertex 3: left-top
    },
    };

    Block getBlock(const ChunkPtr& c, const std::vector<ChunkPtr>& neighbours ,glm::ivec3 pos) {
        ChunkPtr chunk = c;
        if (pos.x < 0) {
            pos.x += CHUNK_SIZE.x;
            chunk = neighbours[1];
        }else if (pos.x >= CHUNK_SIZE.x){
            pos.x -= CHUNK_SIZE.x;
            chunk = neighbours[0];
        }

        if (pos.z < 0) {
            pos.z += CHUNK_SIZE.z;
            chunk = neighbours[3];
        }else if (pos.z >= CHUNK_SIZE.z){
            pos.z -= CHUNK_SIZE.z;
            chunk = neighbours[2];
        }
        if (chunk) {
            return chunk->getBlock(pos);
        }
        return Block::Air;
    }

    inline float calculateAO(
        const ChunkPtr& current,
        const std::vector<ChunkPtr>& neighbours,
        const glm::ivec3& pos,
        const int face,
        const int vertex
    ) {
        const auto& offsets = AO_OFFSETS[face][vertex];
        const bool side1 = getBlock(current, neighbours, pos + offsets[0]) != Block::Air;
        const bool side2 = getBlock(current, neighbours, pos + offsets[1]) != Block::Air;
        const bool corner = getBlock(current, neighbours, pos + offsets[2]) != Block::Air;

        if (side1 && side2) {
            return 0.f;
        }

        return 3.f - static_cast<float>(side1 + side2 + corner);
    }

    ChunkMesh toMesh(const ChunkPtr& current, const std::vector<ChunkPtr>& neighbours) {
        auto mesh = ChunkMesh();
        if (current) {
            for (auto x = 0; x < CHUNK_SIZE.x; x++) {
                for (auto z = 0; z < CHUNK_SIZE.z; z++) {
                    for (auto y = 0; y < CHUNK_SIZE.y; y++) {
                        const auto pos = glm::vec3{x,y,z};
                        const auto block = current->getBlock(pos);
                        if (block == Block::Air) continue;
                        const auto [top, side, bottom] = getMetadata(block);
                        for (auto i = 0; i < 6; i++) {
                            if (getBlock(current, neighbours, pos + DIRECTIONS[i]) == Block::Air) {
                                auto tex = i == 2 ? top : i == 3 ? bottom : side;
                                float ao[4];
                                for (int k = 0; k < 4; k++) {
                                    ao[k] = calculateAO(current, neighbours,pos,i,k);
                                }
                                for (int j = 0; j < 4; j++) {
                                    mesh.vertices.emplace_back(pos + FACES[i][j], glm::vec4(DIRECTIONS[i],ao[j]), TEXTURE_UV[j], tex);
                                }
                                const auto baseVertex = mesh.vertices.size();
                                for (const auto& ind : INDICES) {
                                    mesh.indices.emplace_back(baseVertex + ind);
                                }
                            }
                        }
                    }
                }
            }
        }
        return mesh;
    }


}
