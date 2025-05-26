#include "cube/graphics/Mesher.hpp"

#include "cube/core/Constants.hpp"

namespace cube {

    Mesh toMesh(const ChunkPtr &chunk) {
        Mesh mesh{};
        uint i = 0;
        for (int x = 0; x < CHUNK_WIDTH;x++) {
            for (int z = 0; z < CHUNK_DEPTH;z++) {
                for (int y = 0; y < CHUNK_HEIGHT;y++) {
                    if (chunk->getBlock(x,y,z) == BlockID::Air) {
                        continue;
                    }

                    if (chunk->getBlock(x,y+1,z) == BlockID::Air) {
                        mesh.vertices.insert(mesh.vertices.end(),{
                            Vertex3D{{x  , y  , z  },getColor(chunk->getBlock(x,y,z))},
                            Vertex3D{{x+1, y  , z  },getColor(chunk->getBlock(x,y,z))},
                            Vertex3D{{x+1, y  , z+1},getColor(chunk->getBlock(x,y,z))},
                            Vertex3D{{x  , y  , z+1},getColor(chunk->getBlock(x,y,z))},
                        });

                        mesh.indices.insert(mesh.indices.end(),{
                            i, i+2, i+1,
                            i, i+3, i+2
                        });
                        i += 4;
                    }
                }
            }
        }

        return mesh;
    }
}
