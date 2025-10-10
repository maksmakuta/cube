#ifndef CUBE_RECT_HPP
#define CUBE_RECT_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace cube {

    class Rect {
    public:
        Rect() = default;
        Rect(const glm::vec2& pos, const glm::vec2& size);
        Rect(float x, float y, float w, float h);

        glm::vec2 getPos() const;
        glm::vec2 getSize() const;
        glm::vec4 getRaw() const;

        void setPos(const glm::vec2&);
        void setSize(const glm::vec2&);
        void setRaw(const glm::vec4&);

        bool contains(const glm::vec2&) const;

    private:
        glm::vec4 m_rect;
    };

}

#endif //CUBE_RECT_HPP