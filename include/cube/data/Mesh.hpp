#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP

#include <vector>

#include "cube/graphics/Vertex.hpp"

namespace cube {

    struct Mesh final {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Vertex> tvertices;
        std::vector<uint32_t> tindices;
    };

}

#endif //CUBE_MESH_HPP
