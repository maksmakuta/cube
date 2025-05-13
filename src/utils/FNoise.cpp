#include "cube/utils/FNoise.hpp"

#include <algorithm>
#include <random>
#include <cmath>

namespace cube {

    FNoise::FNoise(const unsigned int seed) {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);
        std::default_random_engine engine(seed == 0 ? std::random_device{}() : seed);
        std::ranges::shuffle(p, engine);
        p.insert(p.end(), p.begin(), p.end());
    }

    float FNoise::fade(const float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float FNoise::lerp(const float t, const float a, const float b) {
        return a + t * (b - a);
    }

    float FNoise::grad(const int hash, const float x, const float y) {
        const int h = hash & 7;
        const float u = h < 4 ? x : y;
        const float v = h < 4 ? y : x;
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
    }

    float FNoise::at(const float x, const float y) const {
        const int xi = static_cast<int>(std::floor(x)) & 255;
        const int yi = static_cast<int>(std::floor(y)) & 255;

        const float xf = x - std::floor(x);
        const float yf = y - std::floor(y);

        const float u = fade(xf);
        const float v = fade(yf);

        const int aa = p[p[xi    ] + yi    ];
        const int ab = p[p[xi    ] + yi + 1];
        const int ba = p[p[xi + 1] + yi    ];
        const int bb = p[p[xi + 1] + yi + 1];

        const float x1 = lerp(u, grad(aa, xf, yf), grad(ba, xf - 1, yf));
        const float x2 = lerp(u, grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1));
        return (lerp(v, x1, x2) + 1.0f) * 0.5f;
    }

}
