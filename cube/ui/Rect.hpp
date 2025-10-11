#ifndef CUBE_RECT_HPP
#define CUBE_RECT_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace cube {

    class Rect {
    public:
        Rect();
        Rect(const glm::vec2& pos, const glm::vec2& size);
        Rect(float x, float y, float w, float h);

        [[nodiscard]] glm::vec2 getPos() const;
        [[nodiscard]] glm::vec2 getSize() const;
        [[nodiscard]] glm::vec4 getRaw() const;

        void setPos(const glm::vec2&);
        void setSize(const glm::vec2&);
        void setRaw(const glm::vec4&);

        [[nodiscard]] bool contains(const glm::vec2&) const;

    private:
        glm::vec4 m_rect{};
    };

}

#endif //CUBE_RECT_HPP