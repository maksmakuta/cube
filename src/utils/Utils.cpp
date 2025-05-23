#include "cube/utils/Utils.hpp"

#include "cube/core/Constants.hpp"

#ifndef CUBE_ASSETS_DIR
#error "CUBE_ASSETS_DIR is not defined, please define assets dir"
#endif

namespace cube {

    std::string getAsset(const std::string& path) {
        return CUBE_ASSETS_DIR + path;
    }

    glm::vec4 toVec4(const Color& c) {
        return {
            static_cast<float>(c >> 16 & 0xFF) / 255.f,
            static_cast<float>(c >> 8 & 0xFF) / 255.f,
            static_cast<float>(c >> 0 & 0xFF) / 255.f,
            static_cast<float>(c >> 24 & 0xFF) / 255.f,
        };
    }

    glm::ivec2 toChunk(const glm::vec3 &p) {
        return glm::floor(glm::vec2{p.x,p.z} / glm::vec2{CHUNK_WIDTH, CHUNK_DEPTH});
    }

    bool inChunk(const glm::ivec3 &vec) {
        return vec.x >= 0 && vec.x < CHUNK_WIDTH &&
               vec.y >= 0 && vec.y < CHUNK_HEIGHT &&
               vec.z >= 0 && vec.z < CHUNK_DEPTH;
    }

    glm::uint hash(const char* str) {
        uint32_t hash = 0x811c9dc5;
        while (*str) {
            hash ^= static_cast<uint8_t>(*str++);
            hash *= 0x01000193;
        }
        return hash;
    }
}
