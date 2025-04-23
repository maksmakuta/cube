
#include "cube/graphics/Renderer.hpp"

#include "cube/utils/AssetsUtils.hpp"
#include "glad/gl.h"

namespace cube {
    Renderer::Renderer() : m_shader(getAsset("/shaders/block.vert"),getAsset("/shaders/block.frag")){

    }

    void Renderer::clear(const Color& hex) {
        const auto v = toVec4(hex);
        glClearColor(v.x,v.y,v.z,v.w);
        glClear(GL_COLOR_BUFFER_BIT);

    }
    void Renderer::resize(int w,int h) {

    }

    void Renderer::draw() {

    }

    void Renderer::applyFont(const Font&, const Color&) {

    }

    void Renderer::text(const std::string& text, const glm::vec2& pos) {

    }
}
