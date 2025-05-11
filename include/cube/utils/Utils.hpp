#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>
#include <iostream>

namespace cube {

    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    inline std::ostream& operator<<(std::ostream& os, const glm::vec4& v) {
        return os << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    }

    inline std::ostream& operator<<(std::ostream& os, const glm::vec3& v) {
        return os << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    }

    inline std::ostream& operator<<(std::ostream& os, const glm::vec2& v) {
        return os << "vec2(" << v.x << ", " << v.y << ")";
    }

    inline std::ostream& operator<<(std::ostream& os, const glm::mat4& m) {
        os << "mat4(\n";
        for (int i = 0; i < 4; ++i)
            os << "  [" << m[i][0] << ", " << m[i][1] << ", " << m[i][2] << ", " << m[i][3] << "]\n";
        return os << ")";
    }

}

#endif //UTILS_HPP
