#ifndef RENDERER_HPP
#define RENDERER_HPP

namespace cube {

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void onCreate();
        void onClear();
        void onResize(int w,int h);

    };

}

#endif //RENDERER_HPP
