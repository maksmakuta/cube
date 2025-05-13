#ifndef FNOISE_HPP
#define FNOISE_HPP

#include <vector>

namespace cube {

    using uchar = unsigned char;

    class FNoise {
    public:
        explicit FNoise(unsigned int seed = 0);
        [[nodiscard]] float at(float x, float y) const;

    private:
        std::vector<uchar> p;

        static float fade(float t);
        static float lerp(float t, float a, float b);
        static float grad(int hash, float x, float y);
    };

}

#endif //FNOISE_HPP
