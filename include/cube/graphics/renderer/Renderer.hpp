#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "cube/graphics/Shader.hpp"
#include "cube/graphics/Texture.hpp"
#include "cube/utils/ColorUtils.hpp"

namespace cube {

    enum LineCap {

    };

    enum LineJoint {

    };

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void onCreate();
        void onClear();
        void onResize(int w, int h);

        void clear(const Color&);
        void fill(const Color&);
        void stroke(const Color&, int w, LineCap c, LineJoint j);
        void image(const Texture&, const glm::vec2&,const glm::vec2&);

        void text(const std::string&, const glm::vec2&);
        void line(float x1, float y1, float x2, float y2);
        void rect(float x, float y, float w, float h);
        void circle(float x, float y, float r);
        void ellipse(float x, float y, float rx, float ry);

    private:
        void flush();

        glm::mat4 m_projection{1.f};
        Shader m_shader;
        uint m_vao{0}, m_vbo{0}, m_ebo{0};
    };

}

#endif //RENDERER_HPP
