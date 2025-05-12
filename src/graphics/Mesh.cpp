#include "cube/graphics/Mesh.hpp"

#include <glm/geometric.hpp>

namespace cube {
    Mesh::Mesh() = default;
    Mesh::~Mesh() = default;

    constexpr glm::ivec3 faceVertexOffsets[6][4] = {
        {  {1, 1, 1}, {1, 0, 1}, {1, 0, 0}, {1, 1, 0} },
        {  {0, 1, 0}, {0, 0, 0}, {0, 0, 1}, {0, 1, 1} },
        {  {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {1, 1, 1} },
        {  {1, 0, 1}, {0, 0, 1}, {0, 0, 0}, {1, 0, 0} },
        {  {0, 1, 1}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1} },
        {  {1, 1, 0}, {1, 0, 0}, {0, 0, 0}, {0, 1, 0} }
    };

    constexpr glm::ivec3 faceNormals[6] = {
        { 1,  0,  0},  //Right
        {-1,  0,  0},  //Left
        { 0,  1,  0},  //Top
        { 0, -1,  0},  //Bottom
        { 0,  0,  1},  //Front
        { 0,  0, -1}   //Back
    };

    constexpr glm::vec2 faceUVs[4] = {
        {0, 0}, {0, 1}, {1, 1}, {1, 0}
    };

    glm::vec2 mix(const BlockID id, const glm::vec2& t, const int face) {
        const glm::vec4 uv = getTile(id, face);
        return glm::mix(glm::vec2{uv.x,uv.y},glm::vec2{uv.z,uv.w},t);
    }

    ChunkMesh Mesh::toMesh(Chunk& c) {
        ChunkMesh mesh{};

        for (int x = 0; x < CHUNK_WIDTH; ++x){
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                for (int z = 0; z < CHUNK_DEPTH; ++z) {
                    const auto block = c.at({x, y, z});
                    if (block == BlockID::Air)
                        continue;

                    for (int face = 0; face < 6; ++face) {
                        if (glm::ivec3 neighbor = glm::ivec3(x, y, z) + faceNormals[face]; c.is(neighbor,BlockID::Air)) {
                            for (const auto i : std::vector{0,2,3,0,1,2}) {
                                Vertex3D v{};
                                v.pos = glm::ivec3(x, y, z) + faceVertexOffsets[face][i];
                                v.tex = mix(block,faceUVs[i],face);
                                mesh.push_back(v);
                            }
                        }
                    }
                }
            }
        }
        return mesh;
    }
}
