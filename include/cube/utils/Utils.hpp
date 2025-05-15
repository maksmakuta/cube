#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <glm/glm.hpp>

namespace cube {

    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    template<class T, class S>
    void set(T& val, S bit, bool op) {
        op ? val |= bit : val &= ~bit;
    }

    /*
     * Color in hex format: 0xAARRGGBB
     */
    using Color = glm::uint;

    std::string getAsset(const std::string& path);

    glm::vec4 toVec4(const Color&);
    glm::vec2 toChunk(const glm::vec3&);

    glm::uint hash(const char*);

}

#endif //UTILS_HPP
