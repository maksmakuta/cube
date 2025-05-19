#include "cube/graphics/Mesher.hpp"

#include <queue>
#include <glm/common.hpp>

namespace cube {

    Mesher::Mesher() = default;
    Mesher::~Mesher() = default;

    constexpr glm::vec3 directions[6] = {
        { 1, 0, 0},
        {-1, 0, 0},
        { 0, 1, 0},
        { 0,-1, 0},
        { 0, 0, 1},
        { 0, 0,-1},
    };

    VoxelMesh Mesher::toMesh(const Chunk& c) {
        std::queue<glm::ivec3> blocks;

        for (int x = 0; x < CHUNK_WIDTH;++x) {
            for (int z = 0; z < CHUNK_DEPTH;++z) {
                for (int y = 0; y < CHUNK_HEIGHT;++y) {
                    if (!c.is({x,y,z},BlockID::Air) && c.is(glm::vec3{x,y+1,z},BlockID::Air)) {
                        blocks.emplace(x,y,z);
                    }
                }
            }
        }

        VoxelMesh mesh{};
        mesh.key = c.getOffset();

        uint32_t i = 0;

        while (!blocks.empty()) {
            const auto pos = blocks.front();
            const auto id = c.get(pos);
            const auto uv = getTile(id,Face::Top);

            const auto temp = std::vector{
                glm::ivec3{0,1,0},
                glm::ivec3{0,1,1},
                glm::ivec3{1,1,1},
                glm::ivec3{1,1,0}
            };

            for (const auto& v : temp) {
                mesh.vertices.emplace_back(
                    glm::vec3(pos + v),
                    glm::mix(glm::vec2{uv.x,uv.y},glm::vec2{uv.z,uv.w},glm::vec2{v.x,v.z})
                );

                mesh.indices.insert(mesh.indices.end(),{
                    i, i + 1, i + 2,
                    i, i + 2, i + 3,
                });
                i += 4;
            }

            blocks.pop();
        }


        return mesh;
    }

}
