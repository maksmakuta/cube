#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

#include <array>

#include "cube/core/Voxel.hpp"
#include "cube/core/Config.hpp"

#include <glm/vec3.hpp>

namespace cube {

    class Chunk final {
    public:
        Chunk();

        Voxel& at(const glm::ivec3& pos);
        [[nodiscard]] Voxel at(const glm::ivec3& pos) const;

    private:
        std::array<Voxel, CHUNK_LEN> voxels;
    };

}

#endif //CUBE_CHUNK_HPP
