#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <glm/vec2.hpp>

#include "Font.hpp"

namespace cube {

    /*
     * Color in hex format: 0xAARRGGBB
     */
    using Color = glm::uint;

    class Renderer {
    public:
        Renderer();

        void clear(const Color& hex = 0xFF101010);

        void draw();

        void applyFont(const Font&, const Color&);
        void text(const std::string& text, const glm::vec2& pos);
    };

}

#endif //RENDERER_HPP
