#include "cube/graphics/renderer/Renderer.hpp"

#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "Polyline2D.h"
#include "cube/utils/AssetsUtils.hpp"
#include "glad/gl.h"

namespace cube {

    namespace internal {

        crushedpixel::Polyline2D::EndCapStyle toCap(const LineCap c) {
            switch (c) {
                case LineCap::Butt:
                    return crushedpixel::Polyline2D::EndCapStyle::BUTT;
                case LineCap::Square:
                    return crushedpixel::Polyline2D::EndCapStyle::SQUARE;
                case LineCap::Round:
                    return crushedpixel::Polyline2D::EndCapStyle::ROUND;
                case LineCap::Loop:
                    return crushedpixel::Polyline2D::EndCapStyle::JOINT;
            }
            return crushedpixel::Polyline2D::EndCapStyle::BUTT;
        }

        crushedpixel::Polyline2D::JointStyle toJoint(const LineJoint j) {
            switch (j) {
                case LineJoint::Miter:
                    return crushedpixel::Polyline2D::JointStyle::MITER;
                case LineJoint::Bevel:
                    return crushedpixel::Polyline2D::JointStyle::BEVEL;
                case LineJoint::Round:
                    return crushedpixel::Polyline2D::JointStyle::ROUND;
            }
            return crushedpixel::Polyline2D::JointStyle::MITER;
        }

        std::vector<glm::vec2> fillMesh(const std::vector<glm::vec2>& path, glm::vec2* min_a, glm::vec2* min_b){
            std::vector<glm::vec2> result;

            glm::vec2 min{std::numeric_limits<float>::max()};
            glm::vec2 max{std::numeric_limits<float>::min()};

            for (int i = 0; i + 1 < path.size();++i) {
                result.push_back(path[0]);
                result.push_back(path[i]);
                result.push_back(path[i+1]);

                min.x = std::min(min.x, path[i].x);
                min.y = std::min(min.y, path[i].y);
                max.x = std::max(max.x, path[i].x);
                max.y = std::max(max.y, path[i].y);
            }

            min.x = std::min(min.x, path.back().x);
            min.y = std::min(min.y, path.back().y);
            max.x = std::max(max.x, path.back().x);
            max.y = std::max(max.y, path.back().y);

            *min_a = min;
            *min_b = max;

            return result;
        }
    }

    Renderer::Renderer() = default;

    Renderer::~Renderer() = default;

    void Renderer::onCreate() {
        m_shader.load(getAsset("/shaders/canvas.vert"),getAsset("/shaders/canvas.frag"));

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), static_cast<const void *>(nullptr));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<const void *>((offsetof(Vertex2D, tex))));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<const void *>((offsetof(Vertex2D, col))));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Renderer::onClear() {
        m_shader.unload();
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
    }

    void Renderer::onResize(const int w, const int h) {
        m_projection = glm::ortho(0.f, static_cast<float>(w),static_cast<float>(h),0.f,-1.f,1.f);
    }

    void Renderer::use() {
        m_shader.use();
    }

    void Renderer::clear(const Color& c) {
        const auto v = toVec4(c);
        glClearColor(v.r,v.g,v.b,v.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::fill(const Color& c) {
        m_paint = Paint::Fill;
        m_color = c;
    }

    void Renderer::stroke(const Color& c, const float w, const LineCap lc, const LineJoint lj) {
        m_paint = Paint::Stroke;
        m_color = c;
        m_width = w;
        m_cap = lc;
        m_joint = lj;
    }

    void Renderer::image(const Texture& t, const glm::vec2& a,const glm::vec2& b) {
        ++m_texture;
        if (m_texture >= 16) {
            m_texture = 0;
        }

        t.bind(m_texture);
        m_paint = Paint::Image;
        m_color = 0xFFFFFFFF;
    }

    void Renderer::text(const std::string&, const glm::vec2&) {

    }

    void Renderer::line(const float x1, const float y1, const float x2, const float y2) {
        if (m_paint == Paint::Stroke) {
            const std::vector<glm::vec2> path{
                {x1,y1},
                {x2,y2}
            };
            flush(path);
        }
    }

    void Renderer::rect(const float x, const float y, const float w, const float h) {
        const std::vector<glm::vec2> path{
                    {x   ,y  },
                    {x+w ,y  },
                    {x+w ,y+h},
                    {x   ,y+h}
        };
        flush(path);
    }

    void Renderer::circle(const float x, const float y, const float r) {
        ellipse(x,y,r,r);
    }

    void Renderer::ellipse(const float x, const float y, const float rx, const float ry) {
        std::vector<glm::vec2> path;
        const auto center = glm::vec2{x,y};
        const auto r = glm::vec2{rx,ry};
        float t = 0.f;
        while(t <= glm::two_pi<float>()) {
            path.push_back(
                center + glm::vec2{cosf(t), sinf(t)} * r
            );
            t += glm::radians(5.f);
        }
        flush(path);
    }

    void Renderer::flush(const std::vector<glm::vec2>& path) {
        auto mesh = std::vector<glm::vec2>();
        glm::vec2 a,b;
        if (m_paint == Paint::Stroke) {
            mesh = crushedpixel::Polyline2D::create(path,m_width,internal::toJoint(m_joint),internal::toCap(m_cap));
        }else {
            mesh = internal::fillMesh(path,&a,&b);
        }

        if (mesh.empty()) {
            std::cout << "1";
            return;
        }

        const auto size = b - a;

        auto vertices = std::vector<Vertex2D>();
        for (const auto item : mesh) {
            Vertex2D vertex{};
            vertex.pos = item;
            vertex.col = toVec4(m_color);

            if (m_paint == Paint::Image) {
                vertex.tex = (item - a) / size;
            }else {
                vertex.tex = glm::vec2{0};
            }

            vertices.push_back(vertex);
        }


        if (vertices.empty()) {
            std::cout << "2";
            return;
        }

//        m_shader.use();
        m_shader.setInt("type",(m_paint == Paint::Image) + 1);
        m_shader.setInt("image",m_texture);
        m_shader.setMat4("proj",m_projection);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<int>(vertices.size()), vertices.data());

        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()));

    }

}
