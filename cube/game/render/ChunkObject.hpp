#ifndef CUBE_CHUNKOBJECT_HPP
#define CUBE_CHUNKOBJECT_HPP

#include <cstdint>

#include "ChunkMesh.hpp"

namespace cube {

    struct ChunkObject {
        uint32_t vao{0};
        uint32_t vbo{0};
        uint32_t ebo{0};
        int count{-1};

        void init(int vertices = 2048, int indices = 4096);
        void update(const ChunkMesh&);
        void release();
    };

    ChunkObject toObject(const ChunkMesh&);

}

#endif //CUBE_CHUNKOBJECT_HPP