#ifndef ICALLBACKS_HPP
#define ICALLBACKS_HPP

namespace cube {

    class ICallbacks {
    public:
        virtual ~ICallbacks() = default;

        virtual void onUpdate(float dt) = 0;
        virtual void onResize(int w, int h) = 0;
        virtual void onKey(int k, int a, int m) = 0;
        virtual void onCursor(float x, float y) = 0;
        virtual void onScroll(float dx, float dy) = 0;
    };

}

#endif //ICALLBACKS_HPP
