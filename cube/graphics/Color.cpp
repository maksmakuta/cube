#include "Color.hpp"

namespace cube {

    Color::Color(const uint32_t rgba) : value(rgba) {}

    Color::Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
        setBytes(r, g, b, a);
    }

    Color::Color(const float r, const float g, const float b, const float a) {
        setFloats(r, g, b, a);
    }

    void Color::setBytes(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
        value =
            (static_cast<uint32_t>(a) << 24) |
            (static_cast<uint32_t>(r) << 16) |
            (static_cast<uint32_t>(g) << 8)  |
            (static_cast<uint32_t>(b));
    }

    void Color::setFloats(const float r, const float g, const float b, const float a) {
        setBytes(
            static_cast<uint8_t>(std::clamp(r, 0.f, 1.f) * 255.f),
            static_cast<uint8_t>(std::clamp(g, 0.f, 1.f) * 255.f),
            static_cast<uint8_t>(std::clamp(b, 0.f, 1.f) * 255.f),
            static_cast<uint8_t>(std::clamp(a, 0.f, 1.f) * 255.f)
        );
    }

    void Color::setUint(const uint32_t rgba) {
        value = rgba;
    }

    uint8_t Color::r() const { return (value >> 16) & 0xFF; }
    uint8_t Color::g() const { return (value >> 8)  & 0xFF; }
    uint8_t Color::b() const { return value & 0xFF; }
    uint8_t Color::a() const { return (value >> 24) & 0xFF; }

    float Color::rf() const { return static_cast<float>(r()) / 255.0f; }
    float Color::gf() const { return static_cast<float>(g()) / 255.0f; }
    float Color::bf() const { return static_cast<float>(b()) / 255.0f; }
    float Color::af() const { return static_cast<float>(a()) / 255.0f; }

    Color::operator uint32_t() const { return value; }

}