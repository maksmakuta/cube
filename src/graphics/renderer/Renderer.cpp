#include "cube/graphics/renderer/Renderer.hpp"

namespace cube {

    Renderer::Renderer() = default;

    Renderer::~Renderer() = default;

    void Renderer::onCreate() {

    }

    void Renderer::onClear() {

    }

    void Renderer::onResize(int w, int h) {

    }

    void Renderer::clear(const Color&) {

    }

    void Renderer::fill(const Color&) {

    }

    void Renderer::stroke(const Color&, int w, LineCap c, LineJoint j) {

    }

    void Renderer::image(const Texture&, const glm::vec2&,const glm::vec2&) {

    }

    void Renderer::text(const std::string&, const glm::vec2&) {

    }

    void Renderer::line(float x1, float y1, float x2, float y2) {

    }

    void Renderer::rect(float x, float y, float w, float h) {

    }

    void Renderer::circle(float x, float y, float r) {

    }

    void Renderer::ellipse(float x, float y, float rx, float ry) {

    }

    void Renderer::flush() {

    }

}