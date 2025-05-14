#include "cube/graphics/renderer/Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "cube/utils/Utils.hpp"

#include "glad/gl.h"
#include "Polyline2D.h"

using namespace crushedpixel;

namespace cube {

    void clear(const Color& c) {
        const auto v = toVec4(c);
        glClearColor(v.r,v.g,v.b,v.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    Renderer::Renderer() = default;
    Renderer::~Renderer() = default;

    void Renderer::onCreate() {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, sizeof(Vertex2D), static_cast<void *>(nullptr));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, tex)));
        glEnableVertexAttribArray(1);
        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(Vertex2D), reinterpret_cast<void *>(offsetof(Vertex2D, col)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_shader.load(
            getAsset("/shaders/canvas.vert"),
            getAsset("/shaders/canvas.frag")
        );

    }

    void Renderer::use() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    void Renderer::onClear() {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        m_shader.unload();
    }

    void Renderer::onResize(const int w, const int h) {
        m_proj = glm::ortho(0.f,static_cast<float>(w),static_cast<float>(h),0.f, 1.f, -1.f);
    }

    void Renderer::flush() {
        int type = 0;
        std::visit(overloaded{
            [&type](const Color&) {
                type = 1;
            },
            [&type](const StrokePaint&) {
                type = 1;
            },
            [&type](const ImagePaint&) {
                type = 2;
            },
            [&type](const TextPaint&) {
                type = 3;
            }
        }, m_paint);

        m_shader.use();
        m_shader.setMat4("proj",m_proj);
        m_shader.setInt("type",type);
        m_shader.setInt("image",0);
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(Vertex2D)), vertices.data(), GL_STREAM_DRAW);
        glDrawArrays(GL_TRIANGLES, 0,static_cast<int>(vertices.size()));

        vertices.clear();
    }

    void Renderer::fill(const Color& c) {
        flush();
        m_paint = c;
    }

    void Renderer::stroke(const Color& c, const float w, const LineCap cap, const LineJoint joint) {
        flush();
        m_paint = StrokePaint(c,w, cap, joint);
    }

    void Renderer::image(const Texture& t, const glm::vec2& uv_a, const glm::vec2& uv_b) {
        flush();
        m_paint = ImagePaint(t,uv_a, uv_b);
    }

    void Renderer::text(const Font &t, const Color& c) {
        flush();
        m_paint = TextPaint(t,c);
    }

    void Renderer::lines(const std::vector<glm::vec2> &path) {
        if (std::holds_alternative<StrokePaint>(m_paint)) {
            process(path);
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
        while (t < angle_b) {
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

    void Renderer::print(const glm::vec2& pos,const std::string& text, const Align& align) {
        if (const auto text_style = std::get_if<TextPaint>(&m_paint)) {
            glm::vec2 p = pos;
            if (align == Align::Center) {
                p.x -= static_cast<float>(text_style->font.width(text)) / 2.f;
            }
            if (align == Align::End) {
                p.x -= static_cast<float>(text_style->font.width(text));
            }

            for(const auto& c : text) {
                if (const auto g = text_style->font.at(c); g.has_value()) {

                    const float x = p.x + static_cast<float>(g->offset.x);
                    const float y = p.y - static_cast<float>(g->offset.y);
                    const float w = g->size.x;
                    const float h = g->size.y;

                    vertices.insert(vertices.end(),{
                        Vertex2D(glm::vec2{x  ,y  },glm::vec2{g->uv_a.x,g->uv_a.y} * static_cast<float>(0xFFFF), text_style->color),
                        Vertex2D(glm::vec2{x+w,y  },glm::vec2{g->uv_b.x,g->uv_a.y} * static_cast<float>(0xFFFF), text_style->color),
                        Vertex2D(glm::vec2{x+w,y+h},glm::vec2{g->uv_b.x,g->uv_b.y} * static_cast<float>(0xFFFF), text_style->color),

                        Vertex2D(glm::vec2{x  ,y  },glm::vec2{g->uv_a.x,g->uv_a.y} * static_cast<float>(0xFFFF), text_style->color),
                        Vertex2D(glm::vec2{x+w,y+h},glm::vec2{g->uv_b.x,g->uv_b.y} * static_cast<float>(0xFFFF), text_style->color),
                        Vertex2D(glm::vec2{x  ,y+h},glm::vec2{g->uv_a.x,g->uv_b.y} * static_cast<float>(0xFFFF), text_style->color)
                    });

                    p.x += static_cast<float>(g->advance);
                }
            }
            process({});
        }
    }

    void Renderer::process(const std::vector<glm::vec2>& path) {
        std::visit(overloaded{
            [this, &path](const Color& fill) {
                for (auto i = 1; i + 1 < path.size(); ++i) {
                    vertices.emplace_back(path[0],glm::vec2{0,0},fill);
                    vertices.emplace_back(path[i],glm::vec2{0,0},fill);
                    vertices.emplace_back(path[i+1],glm::vec2{0,0},fill);
                }
            },
            [this, &path](const StrokePaint& stroke) {
                const auto mesh = Polyline2D::create(
                    path,
                    stroke.width,
                    static_cast<Polyline2D::JointStyle>(stroke.joint),
                    static_cast<Polyline2D::EndCapStyle>(stroke.cap)
                );
                for (const auto& i : mesh) {
                    vertices.emplace_back(i,glm::vec2{0,0},stroke.color);
                }
            },
            [this, &path](const ImagePaint& image) {
                auto min = glm::vec2(std::numeric_limits<float>::max());

                for (const auto& p : path) {
                    min.x = std::min(min.x, p.x);
                    min.y = std::min(min.y, p.y);
                }

                for (auto i = 1; i + 1 < path.size(); ++i) {
                    vertices.emplace_back(path[0],mix(image.uv_min,image.uv_max,path[0] - min),0xFFFFFFFF);
                    vertices.emplace_back(path[i],mix(image.uv_min,image.uv_max,path[i] - min),0xFFFFFFFF);
                    vertices.emplace_back(path[i+1],mix(image.uv_min,image.uv_max,path[i+1] - min),0xFFFFFFFF);
                }
                image.image.bind(0);
            },
            [this](const TextPaint& text) {
                text.font.getTexture().bind(0);
            }
        }, m_paint);
    }
}
