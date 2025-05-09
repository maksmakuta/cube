#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "cube/graphics/Shader.hpp"

namespace cube {

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void onCreate();
        void onClear();
        void onResize(int w,int h);

        void draw();
    private:
        Shader m_shader;
        unsigned int VBO{0}, VAO{0};
    };

}

#endif //RENDERER_HPP
