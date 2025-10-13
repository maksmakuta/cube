#include "Renderer2D.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <functional>
#include <iostream>
#include <optional>

#include "utils/AssetsPaths.hpp"

namespace cube {

    constexpr auto maxMiterLength = 50.f;

    Renderer2D::Renderer2D() {
        m_state.init();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    Renderer2D::~Renderer2D() {
        m_state.release();
    }

    void Renderer2D::resize(const glm::vec2& view) {
        const auto size = glm::ivec2(view);
        m_state.resize(size.x, size.y);
    }

    void Renderer2D::begin() {
        m_state.vertices.clear();
    }

    void Renderer2D::end() {
        m_state.flush();
    }

    void Renderer2D::point(const glm::vec2& center){
        rect(center - glm::vec2(0.5f), glm::vec2(1.f));
    }

    void Renderer2D::line(const glm::vec2& a, const glm::vec2& b){
        toStroke({a,b});
    }

    void Renderer2D::triangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c){
        const auto path_data = {a,b,c};
        path(path_data);
    }

    void Renderer2D::quad(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d){
        const auto path_data = {a,b,c,d};
        path(path_data);
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size){
        quad(
            origin,
            origin + glm::vec2(size.x, 0.f),
            origin + size,
            origin + glm::vec2(0.f, size.y)
        );
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const float r){
        rect(origin, size, glm::vec4(r));
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec2& r){
        rect(origin, size, glm::vec4(r.x, r.y, r.x, r.y));
    }

    void subArc(std::vector<glm::vec2>& path, const glm::vec2& center, const glm::vec2& r, const glm::vec2& range) {
        constexpr int segments = 8; // Segments per corner arc
        const float step = (range.y - range.x) / segments;

        for (int i = 0; i <= segments; ++i) {
            const float angle = range.x + static_cast<float>(i) * step;
            path.emplace_back(center + glm::vec2(std::cos(angle), std::sin(angle)) * r);
        }
    }

    void Renderer2D::rect(const glm::vec2& origin, const glm::vec2& size, const glm::vec4& r){
        const glm::vec4 radii = glm::min(r, glm::vec4(glm::min(size.x, size.y) * 0.5f));

        std::vector<glm::vec2> path_data;
        path_data.reserve(64);

        if (radii.x > 0.f) {
            subArc(path_data, origin + glm::vec2(radii.x, radii.x),
                   glm::vec2(radii.x), glm::vec2(glm::pi<float>(), glm::half_pi<float>() * 3.f));
        } else {
            path_data.emplace_back(origin);
        }

        if (radii.y > 0.f) {
            subArc(path_data, origin + glm::vec2(size.x - radii.y, radii.y),
                   glm::vec2(radii.y), glm::vec2(glm::half_pi<float>() * 3.f, glm::two_pi<float>()));
        } else {
            path_data.emplace_back(origin + glm::vec2(size.x, 0.f));
        }

        if (radii.z > 0.f) {
            subArc(path_data, origin + glm::vec2(size.x - radii.z, size.y - radii.z),
                   glm::vec2(radii.z), glm::vec2(0.f, glm::half_pi<float>()));
        } else {
            path_data.emplace_back(origin + size);
        }

        if (radii.w > 0.f) {
            subArc(path_data, origin + glm::vec2(radii.w, size.y - radii.w),
                   glm::vec2(radii.w), glm::vec2(glm::half_pi<float>(), glm::pi<float>()));
        } else {
            path_data.emplace_back(origin + glm::vec2(0.f, size.y));
        }
        path(path_data);
    }

    void Renderer2D::circle(const glm::vec2& center, const float r){
        ellipse(center, glm::vec2(r));
    }

    void Renderer2D::ellipse(const glm::vec2& center, const glm::vec2& r){
        constexpr int segments = 32;
        constexpr float step = glm::two_pi<float>() / segments;
        auto path_data = std::vector<glm::vec2>{};
        for (int i = 0; i < segments; ++i) {
            const float t0 = static_cast<float>(i) * step;
            glm::vec2 p0 = center + glm::vec2(std::cos(t0), std::sin(t0)) * r;
            path_data.emplace_back(p0);
        }
        path(path_data);
    }

    void Renderer2D::arc(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range){
        constexpr int segments = 32;
        const float step = (range.y - range.x) / segments;
        auto path_data = std::vector<glm::vec2>{};
        for (int i = 0; i < segments; ++i) {
            const float t0 = static_cast<float>(i    ) * step + range.x;
            glm::vec2 p0 = center + glm::vec2(std::cos(t0), std::sin(t0)) * r;
            path_data.emplace_back(p0);
        }
        path(path_data);
    }

    void Renderer2D::pie(const glm::vec2& center, const glm::vec2& r, const glm::vec2& range){
        constexpr int segments = 32;
        const float step = (range.y - range.x) / segments;
        auto path_data = std::vector<glm::vec2>{};
        path_data.emplace_back(center);
        for (int i = 0; i < segments; ++i) {
            const float t0 = static_cast<float>(i    ) * step + range.x;
            glm::vec2 p0 = center + glm::vec2(std::cos(t0), std::sin(t0)) * r;
            path_data.emplace_back(p0);
        }
        path(path_data);
    }

    void Renderer2D::path(const std::vector<glm::vec2>& p) {
        if (m_state.current_state == State::Fill || m_state.current_state == State::Image)
            toFill(p);
        if (m_state.current_state == State::Stroke)
            toStroke(p);
    }

    void Renderer2D::print(const std::string& str, const glm::vec2& pos){
        const auto&[color, font] = m_state.text_state;
        glm::vec2 point = pos;
        for (const auto& c : str) {
            if (const auto g = font.getGlyph(c)) {
                auto x = point.x + g->bearing.x;
                auto y = point.y - g->bearing.y;
                auto w = g->size.x;
                auto h = g->size.y;

                auto p00 = glm::vec2(x  , y  );
                auto p01 = glm::vec2(x  , y+h);
                auto p11 = glm::vec2(x+w, y+h);
                auto p10 = glm::vec2(x+w, y  );

                auto u00 = glm::vec2(g->uv_min.x,g->uv_min.y);
                auto u01 = glm::vec2(g->uv_min.x,g->uv_max.y);
                auto u11 = glm::vec2(g->uv_max.x,g->uv_max.y);
                auto u10 = glm::vec2(g->uv_max.x,g->uv_min.y);

                push(p00, u00);
                push(p01, u01);
                push(p10, u10);
                push(p10, u10);
                push(p01, u01);
                push(p11, u11);

                point.x += static_cast<float>(g->advance);
            }
        }
    }

    void Renderer2D::fill(const uint32_t argb) {
        fill(Color(argb));
    }

    void Renderer2D::fill(const Color& c){
        setState(State::Fill);
        m_state.fill_state.color = c;
    }

    void Renderer2D::fill(const Texture& t,const glm::vec2& uv_min, const glm::vec2& uv_max){
        setState(State::Image);
        m_state.image_state.texture = t;
        m_state.image_state.texture_uv = glm::vec4(uv_min, uv_max);
    }

    void Renderer2D::stroke(const Color& c, const float w, const bool loop){
        setState(State::Stroke);
        m_state.stroke_state.color = c;
        m_state.stroke_state.thickness = w / 2.f;
        m_state.stroke_state.is_loop = loop;
    }

    void Renderer2D::text(const Font& f, const Color& c) {
        setState(State::Text);
        m_state.text_state.color = c;
        m_state.text_state.font = f;
    }

    void Renderer2D::setJoin(const JoinType j){
        m_state.stroke_state.join = j;
    }

    void Renderer2D::setCap(const CapType c){
        m_state.stroke_state.cap = c;
    }

    void Renderer2D::setState(const State s) {
        if (m_state.current_state != s)
            m_state.flush();
        m_state.current_state = s;
    }

    void Renderer2D::push(const glm::vec2& vertex, const glm::vec2& uv) {
        uint32_t col = 0xFFFFFFFF;
        if (m_state.current_state == State::Fill) {
            col = static_cast<uint32_t>(m_state.fill_state.color);
        }
        if (m_state.current_state == State::Stroke) {
            col = static_cast<uint32_t>(m_state.stroke_state.color);
        }
        if (m_state.current_state == State::Text) {
            col = static_cast<uint32_t>(m_state.text_state.color);
        }
        m_state.vertices.emplace_back(vertex, uv, col);
    }

    void Renderer2D::toFill(const std::vector<glm::vec2>& path) {
        if (m_state.current_state == State::Fill) {
            for (size_t i = 1; i + 1 < path.size(); ++i) {
                push(path.front());
                push(path[i]);
                push(path[i + 1]);
            }
            return;
        }
        calcBox(path);
        const auto img_state = m_state.image_state;
        const float minX = img_state.texture_box.x;
        const float minY = img_state.texture_box.y;
        const float maxX = img_state.texture_box.z;
        const float maxY = img_state.texture_box.w;

        auto computeUV = [&](const glm::vec2& v) -> glm::vec2 {
            float u_ = (v.x - minX) / (maxX - minX);
            float v_ = (v.y - minY) / (maxY - minY);
            return {u_, v_};
        };

        for (size_t i = 1; i + 1 < path.size(); ++i) {
            push(path.front(), computeUV(path.front()));
            push(path[i], computeUV(path[i]));
            push(path[i + 1], computeUV(path[i + 1]));
        }
    }

    void Renderer2D::calcBox(const std::vector<glm::vec2> &path) {
        if (path.empty()) return;

        float minX = path[0].x;
        float maxX = path[0].x;
        float minY = path[0].y;
        float maxY = path[0].y;

        for (const auto& p : path) {
            if (p.x < minX) minX = p.x;
            if (p.x > maxX) maxX = p.x;
            if (p.y < minY) minY = p.y;
            if (p.y > maxY) maxY = p.y;
        }

        m_state.image_state.texture_box = glm::vec4(minX, minY, maxX, maxY);
    }

    struct Line {

        Line(const glm::vec2& start, const glm::vec2& end) : a(start), b(end){}

        Line operator + (const glm::vec2& v) const {
            return {a + v, b + v};
        }

        Line operator - (const glm::vec2& v) const {
            return {a - v, b - v};
        }

        [[nodiscard]] glm::vec2 dir(const bool n = true) const {
            const auto d = b - a;
            if (n) return glm::normalize(d);
            return d;
        }

        [[nodiscard]] glm::vec2 normal() const {
            const auto d = dir();
            return {-d.y, d.x};
        }

        glm::vec2 a,b;
    };

    std::optional<glm::vec2> intersect(const Line& l1, const Line& l2, const bool infinite = false) {
        const auto p = l1.a;
        const auto r = l1.b - l1.a;
        const auto q = l2.a;
        const auto s = l2.b - l2.a;

        const auto rxs = r.x * s.y - r.y * s.x;
        if (std::abs(rxs) < 1e-6f)
            return std::nullopt;

        const auto qp = q - p;
        const auto t = (qp.x * s.y - qp.y * s.x) / rxs;
        const auto u = (qp.x * r.y - qp.y * r.x) / rxs;

        if (!infinite && (t < 0.f || t > 1.f || u < 0.f || u > 1.f)){
            return std::nullopt;
        }

        return p + t * r;
    }

    struct Segment {

        Segment(const Line& middle, const float thick) : top(middle + middle.normal() * thick),
            bottom(middle - middle.normal() * thick) , center(middle){}

        Line top;
        Line bottom;
        Line center;
    };

    void fan(
        std::vector<Vertex2D>& vertices,
        const glm::vec2& center,
        const glm::vec2& connect,
        const glm::vec2& from,
        const glm::vec2& to,
        const bool clockwise,
        const glm::uint& color
    ) {
        const auto point1 = from - center;
        const auto point2 = to - center;
        auto angle1 = atan2f(point1.y, point1.x);
        auto angle2 = atan2f(point2.y, point2.x);

        if (clockwise) {
            if (angle2 > angle1) {
                angle2 = angle2 - glm::two_pi<float>();
            }
        } else {
            if (angle1 > angle2) {
                angle1 = angle1 - glm::two_pi<float>();
            }
        }

        const auto jointAngle = angle2 - angle1;
        const auto numTriangles = std::max(1, static_cast<int>(std::floor(std::abs(jointAngle) / 0.174533)));
        const auto triAngle = jointAngle / static_cast<float>(numTriangles);

        auto startPoint = from;
        glm::vec2 endPoint;
        for (int t = 0; t < numTriangles; t++) {
            if (t + 1 == numTriangles) {
                endPoint = to;
            } else {
                const auto rot = static_cast<float>(t + 1) * triAngle;
                endPoint.x = std::cosf(rot) * point1.x - std::sinf(rot) * point1.y;
                endPoint.y = std::sinf(rot) * point1.x + std::cosf(rot) * point1.y;
                endPoint = endPoint + center;
            }

            vertices.emplace_back(startPoint, glm::vec2{-1,-1}, color);
            vertices.emplace_back(endPoint, glm::vec2{-1,-1}, color);
            vertices.emplace_back(connect, glm::vec2{-1,-1}, color);

            startPoint = endPoint;
        }
    }

    void Renderer2D::toStroke(const std::vector<glm::vec2>& path) {
        if (path.size() < 2) return;

        auto& state = m_state.stroke_state;
        const auto color = static_cast<uint32_t>(state.color);

        auto segments = std::vector<Segment>();
        for (auto i = 0; i + 1 < path.size(); ++i) {
            segments.emplace_back(Line(path[i], path[i+1]), state.thickness);
        }

        if (state.is_loop) {
            segments.emplace_back(Line(path.back(), path.front()), state.thickness);
        }else {
            if (state.cap == CapType::Round) {
                fan(m_state.vertices, segments.back().center.b, segments.back().center.b,
                    segments.back().top.b, segments.back().bottom.b, true, color);

                fan(m_state.vertices, segments.front().center.a, segments.front().center.a,
                    segments.front().bottom.a, segments.front().top.a, true, color);
            }
            if (state.cap == CapType::Square) {
                segments.back().top.b += segments.back().center.dir() * state.thickness;
                segments.back().bottom.b += segments.back().center.dir() * state.thickness;
                segments.front().top.a -= segments.front().center.dir() * state.thickness;
                segments.front().bottom.a -= segments.front().center.dir() * state.thickness;
            }
        }

        auto onSegmentPair = [this, &state, color](Segment& s1, Segment& s2) {
            const auto dir1 = s1.center.dir();
            const auto dir2 = s2.center.dir();

            const auto _angle = glm::angle(dir1, dir2);

            auto wrappedAngle = _angle;
            if (wrappedAngle > glm::half_pi<float>()) {
                wrappedAngle = glm::pi<float>() - wrappedAngle;
            }

            if (state.join == JoinType::Miter && wrappedAngle < 0.349066) {
                state.join = JoinType::Bevel;
            }

            const glm::vec2 d1 = s1.center.dir();
            const glm::vec2 d2 = s2.center.dir();
            const auto _left = d1.x * d2.y - d1.y * d2.x;
            const bool left = _left > 0.f;

            Line& outer1 = left ? s1.top : s1.bottom;
            Line& inner1 = left ? s1.bottom : s1.top;
            Line& outer2 = left ? s2.top : s2.bottom;
            Line& inner2 = left ? s2.bottom : s2.top;

            if (const auto p = intersect(outer1, outer2,true)) {
                outer1.b = *p;
                outer2.a = *p;
            }

            if (state.join == JoinType::Round) {
                fan(m_state.vertices, s1.center.b, outer1.b,
                    inner1.b, inner2.a, !left, color);
            }else if (state.join == JoinType::Miter) {
                if (const auto p = intersect(inner1, inner2, true)) {
                    inner1.b = *p;
                    inner2.a = *p;
                }
            }else{
                push(outer1.b);
                push(inner1.b);
                push(inner2.a);
            }
        };

        if (state.is_loop) {
            onSegmentPair(segments.back(),segments.front());
        }
        for (auto i = 0; i + 1 < segments.size(); ++i) {
            auto& s1 = segments[i];
            auto& s2 = segments[i + 1];
            onSegmentPair(s1,s2);
        }

        for (const auto& s : segments) {
            push(s.top.a);
            push(s.top.b);
            push(s.bottom.a);

            push(s.top.b);
            push(s.bottom.a);
            push(s.bottom.b);
        }

    }

}
