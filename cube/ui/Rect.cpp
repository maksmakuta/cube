#include "Rect.hpp"

namespace cube {
    Rect::Rect() = default;

    Rect::Rect(const glm::vec2& pos, const glm::vec2& size) : m_rect(pos,size) {}
    Rect::Rect(const float x, const float y, const float w, const float h) : m_rect(x,y,w,h) {}

    glm::vec2 Rect::getPos() const {
        return {m_rect.x, m_rect.y};
    }

    glm::vec2 Rect::getSize() const {
        return {m_rect.z, m_rect.w};
    }

    glm::vec4 Rect::getRaw() const {
        return m_rect;
    }

    void Rect::setPos(const glm::vec2& p) {
        m_rect.x = p.x;
        m_rect.y = p.y;
    }

    void Rect::setSize(const glm::vec2& s) {
        m_rect.z = s.x;
        m_rect.w = s.y;
    }

    void Rect::setRaw(const glm::vec4& r) {
        m_rect = r;
    }

    bool Rect::contains(const glm::vec2& v) const {
        const auto p = getPos();
        const auto s = getSize();
        return v.x >= p.x && v.x <= p.x + s.x &&
               v.y >= p.y && v.y <= p.y + s.y;
    }

}