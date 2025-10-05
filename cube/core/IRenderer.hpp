#ifndef CUBE_IRENDERER_HPP
#define CUBE_IRENDERER_HPP

namespace cube {

    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual void beginDraw() = 0;
        virtual void endDraw() = 0;
    };

}

#endif //CUBE_IRENDERER_HPP