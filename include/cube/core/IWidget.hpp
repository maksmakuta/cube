#ifndef IWIDGET_HPP
#define IWIDGET_HPP

#include <glm/vec2.hpp>

namespace cube {

    class Renderer;

    class IWidget {
    public:
        virtual ~IWidget() = default;

        virtual void onCreate() = 0;
        virtual void onClear() = 0;
        virtual void onDraw(Renderer&) = 0;
        virtual void onUpdate(float dt) = 0;

        virtual void onResize(int w,int h) = 0;
        virtual void onKey(int k, int a, int m) = 0;
        virtual void onCursor(float x,float y) = 0;
        virtual void onText(unsigned int c) = 0;

        [[nodiscard]] glm::vec2 getSize() const {
            return m_size;
        }

        [[nodiscard]] glm::vec2 getPos() const {
            return m_pos;
        }

        void setPos(const glm::vec2& p) {
            m_pos = p;
        }

        void setSize(const glm::vec2& s) {
            m_size = s;
        }

    private:
        glm::ivec2 m_pos{0};
        glm::ivec2 m_size{0};
    };

}

#endif //IWIDGET_HPP
