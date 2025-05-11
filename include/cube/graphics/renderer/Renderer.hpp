#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <variant>

#include "Vertex.hpp"
#include "cube/graphics/Font.hpp"
#include "cube/graphics/Shader.hpp"
#include "cube/graphics/Texture.hpp"
#include "cube/utils/ColorUtils.hpp"

namespace cube {

    enum class LineJoint {
        Miter,
        Bevel,
        Round
    };

    enum class LineCap {
        Butt,
        Square,
        Round,
        Loop
    };

    struct StrokePaint {
        Color color;
        float width;
        LineCap cap;
        LineJoint joint;
    };

    struct ImagePaint {
        Texture image;
        glm::vec2 uv_min;
        glm::vec2 uv_max;
    };

    struct TextPaint {
        Font font;
        Color color;
    };

    using Paint = std::variant<Color,StrokePaint,ImagePaint,TextPaint>;

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void onCreate();
        void onClear();
        void onResize(int w,int h);

        void flush();

        void fill(const Color&);
        void stroke(const Color&, float w, LineCap cap = LineCap::Butt, LineJoint joint = LineJoint::Miter);
        void image(const Texture& t, const glm::vec2& uv_a = {0,0},const glm::vec2& uv_b = {1,1});
        void text(const Font& t, const Color& c);

        void line(const glm::vec2& a,const glm::vec2& b);
        void lines(const std::vector<glm::vec2>& path);
        void bezier(const glm::vec2& a,const glm::vec2& b,const glm::vec2& c);
        void bezier(const glm::vec2& a,const glm::vec2& b,const glm::vec2& c,const glm::vec2& d);
        void triangle(const glm::vec2& a,const glm::vec2& b,const glm::vec2& c);
        void quad(const glm::vec2& a,const glm::vec2& b,const glm::vec2& c,const glm::vec2& d);

        void line(float x1,float y1,float x2,float y2);
        void bezier(float x1,float y1,float x2,float y2,float x3,float y3);
        void bezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);
        void triangle(float x1,float y1,float x2,float y2,float x3,float y3);
        void quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);

        void rect(float x, float y, float w, float h);
        void rect(float x, float y, float w, float h, float r);
        void circle(float x, float y,float r);
        void ellipse(float x, float y,float rx,float ry);
        void arc(float x, float y,float rx,float ry, float angle_a, float angle_b, bool ccw = false);
        void segment(float x, float y,float rx,float ry, float angle_a, float angle_b, bool ccw = false);

        void print(const glm::vec2& pos, const std::string& text, const Align& align = Align::Start);

    private:
        void process(const std::vector<glm::vec2>&);

        glm::mat4 m_proj{1.f};
        std::vector<Vertex2D> vertices;
        Paint m_paint = 0xFFFFFFFF;
        Shader m_shader;
        uint m_vao{0};
        uint m_vbo{0};
    };

    void clear(const Color&);

}

#endif //RENDERER_HPP
