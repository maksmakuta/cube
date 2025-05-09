#include "cube/graphics/renderer/Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "cube/utils/AssetsUtils.hpp"
#include "glad/gl.h"

namespace cube {

    Renderer::Renderer() = default;
    Renderer::~Renderer() = default;

    void Renderer::onCreate() {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1024 * sizeof(uint), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, tex)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, col)));
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_shader.load(
            getAsset("/shaders/canvas.vert"),
            getAsset("/shaders/canvas.frag")
        );
    }

    void Renderer::onClear() {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
        m_shader.unload();
    }

    void Renderer::onResize(const int w, const int h) {
        glViewport(0,0,w,h);
        m_proj = glm::ortho(0.f,static_cast<float>(w),static_cast<float>(h),0.f, 1.f, -1.f);
    }

    void Renderer::flush() {
        m_shader.use();
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(Vertex2D)), vertices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(uint)), indices.data(), GL_DYNAMIC_DRAW);

        glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);

        vertices.clear();
        indices.clear();
    }

    void Renderer::fill(const Color& c) {
        m_paint = c;
    }

    void Renderer::stroke(const Color& c, const float w, const LineCap cap, const LineJoint joint) {
        m_paint = StrokePaint(c,w, cap, joint);
    }

    void Renderer::image(const Texture& t, const glm::vec2& uv_a, const glm::vec2& uv_b) {
        m_paint = ImagePaint(t,uv_a, uv_b);
    }

    void Renderer::text(const Font &t, const Color& c, const Align &align) {
        m_paint = TextPaint(t,c,align);
    }

    void Renderer::lines(const std::vector<glm::vec2> &path) {
        if (std::holds_alternative<StrokePaint>(m_paint)) {

        }
    }

    void Renderer::line(const glm::vec2& a,const glm::vec2& b) {
        if (std::holds_alternative<StrokePaint>(m_paint)) {
            lines({a, b});
        }
    }

    void Renderer::bezier(const glm::vec2& a,const glm::vec2& b,const glm::vec2& c) {
        if (std::holds_alternative<StrokePaint>(m_paint)) {
            auto path = std::vector<glm::vec2>();
            path.push_back(a);
            float t = 0.1f;
            while (t < 1.f) {
                const auto t1 = mix(a,b,t);
                const auto t2 = mix(b,c,t);
                path.push_back(mix(t1,t2,t));
                t += 0.1f;
            }
            path.push_back(c);
            lines(path);
        }
    }

    void Renderer::bezier(const glm::vec2& a,const glm::vec2& b,const glm::vec2& c,const glm::vec2& d) {
        if (std::holds_alternative<StrokePaint>(m_paint)) {
            auto path = std::vector<glm::vec2>();
            path.push_back(a);
            float t = 0.1f;
            while (t < 1.f) {
                const auto t1 = mix(a,b,t);
                const auto t2 = mix(b,c,t);
                const auto t3 = mix(c,d,t);

                const auto r1 = mix(t1,t2,t);
                const auto r2 = mix(t2,t3,t);

                path.push_back(mix(r1,r2,t));
                t += 0.1f;
            }
            path.push_back(d);
            lines(path);
        }
    }

    void Renderer::triangle(const glm::vec2& a,const glm::vec2& b,const glm::vec2& c) {
        process({a,b,c});
    }

    void Renderer::quad(const glm::vec2& a,const glm::vec2& b,const glm::vec2& c,const glm::vec2& d) {
        process({a,b,c,d});
    }

    void Renderer::line(float x1,float y1,float x2,float y2) {
        line({x1,y1},{x2,y2});
    }

    void Renderer::bezier(float x1,float y1,float x2,float y2,float x3,float y3) {
        bezier({x1,y1},{x2,y2},{x3,y3});
    }

    void Renderer::bezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4) {
        bezier({x1,y1},{x2,y2},{x3,y3},{x4,y4});
    }

    void Renderer::triangle(float x1,float y1,float x2,float y2,float x3,float y3) {
        triangle({x1,y1},{x2,y2},{x3,y3});
    }

    void Renderer::quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4) {
        quad({x1,y1},{x2,y2},{x3,y3},{x4,y4});
    }

    void Renderer::rect(float x, float y, float w, float h) {
        quad(
            {x  ,y  },
            {x+w,y  },
            {x+w,y+h},
            {x  ,y+h}
        );
    }

    void Renderer::rect(float x, float y, float w, float h, float r) {

    }

    void Renderer::circle(const float x, const float y, const float r) {
        ellipse(x,y,r,r);
    }

    void Renderer::ellipse(const float x, const float y, const float rx, const float ry) {
        arc(x,y,rx,ry,0, glm::two_pi<float>());
    }

    void Renderer::arc(const float x, const float y, const float rx, const float ry, const float angle_a, const float angle_b, bool ccw) {
        auto path = std::vector<glm::vec2>();
        const auto center = glm::vec2{x,y};
        const auto r = glm::vec2{rx,ry};
        float t = angle_a;
        while (t <= angle_b) {
            const auto unit = glm::vec2{cosf(t),sinf(t)};
            path.push_back(center + unit * r);

            t += glm::radians(5.f);
            if (t > angle_b) {
                t = angle_b;
            }
        }
        process(path);
    }

    void Renderer::segment(const float x, const float y, const float rx, const float ry, const float angle_a, const float angle_b, bool ccw) {
        auto path = std::vector<glm::vec2>();
        const auto center = glm::vec2{x,y};
        const auto r = glm::vec2{rx,ry};
        path.push_back(center);
        float t = angle_a;
        while (t <= angle_b) {
            const auto unit = glm::vec2{cosf(t),sinf(t)};
            path.push_back(center + unit * r);

            t += glm::radians(5.f);
            if (t > angle_b) {
                t = angle_b;
            }
        }
        process(path);
    }

    void Renderer::print(const glm::vec2& pos,const std::string& text) {

    }

    void Renderer::process(const std::vector<glm::vec2>& path) {

    }
}
