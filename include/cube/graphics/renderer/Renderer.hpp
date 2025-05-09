#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

#include "cube/graphics/Shader.hpp"
#include "cube/graphics/Texture.hpp"
#include "cube/utils/ColorUtils.hpp"

namespace cube {

    enum class LineCap : uint8_t{
        Butt,
        Square,
        Round,
        Loop
    };

    enum class LineJoint : uint8_t{
        Miter,
        Bevel,
        Round
    };

    enum class Paint : uint8_t{
        Fill,
        Stroke,
        Image
    };

    struct Vertex2D {
        glm::vec2 pos{0};
        glm::vec2 tex{0};
        glm::vec4 col{0};
    };

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void onCreate();
        void onClear();
        void onResize(int w, int h);

        void use();
        void fill(const Color&);
        void stroke(const Color&, float w, LineCap lc, LineJoint lj);
        void image(const Texture&, const glm::vec2&,const glm::vec2&);

        void text(const std::string&, const glm::vec2&);
        void line(float x1, float y1, float x2, float y2);
        void rect(float x, float y, float w, float h);
        void circle(float x, float y, float r);
        void ellipse(float x, float y, float rx, float ry);

        static void clear(const Color&);

    private:
        void flush(const std::vector<glm::vec2>&);

        glm::mat4 m_projection{1.f};
        Shader m_shader;
        Color m_color{0};
        uint m_vao{0};
        uint m_vbo{0};
        float m_width = 1.f;
        uint8_t m_texture{0};
        LineCap m_cap = LineCap::Butt;
        LineJoint m_joint = LineJoint::Miter;
        Paint m_paint = Paint::Fill;
    };

}

#endif //RENDERER_HPP
