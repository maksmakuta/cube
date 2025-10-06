#ifndef CUBE_COLOR_HPP
#define CUBE_COLOR_HPP

#include <cstdint>

namespace cube {

    class Color {
    public:
        Color() = default;
        explicit Color(uint32_t rgba);
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        Color(float r, float g, float b, float a = 1.0f);

        void setBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        void setFloats(float r, float g, float b, float a = 1.0f);
        void setUint(uint32_t rgba);

        [[nodiscard]] uint8_t r() const;
        [[nodiscard]] uint8_t g() const;
        [[nodiscard]] uint8_t b() const;
        [[nodiscard]] uint8_t a() const;

        [[nodiscard]] float rf() const;
        [[nodiscard]] float gf() const;
        [[nodiscard]] float bf() const;
        [[nodiscard]] float af() const;

        [[nodiscard]] explicit operator uint32_t() const;

    private:
        uint32_t value = 0xFFFFFFFF;
    };

}

#endif //CUBE_COLOR_HPP