#ifndef GAME_HPP
#define GAME_HPP

namespace cube {

    class IGame {
        public:
        virtual ~IGame() = default;

        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void draw() = 0;
        virtual void update(float delta) = 0;

        virtual void onResize(int w,int h) = 0;
        virtual void onCursor(float x,float y) = 0;
        virtual void onKey(int key,int action, int mods) = 0;
    };

}

#endif //GAME_HPP
