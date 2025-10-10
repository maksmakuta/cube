#ifndef CUBE_LAYOUTPARAMS_HPP
#define CUBE_LAYOUTPARAMS_HPP

namespace cube {

    enum class SizeMode {
        Fixed,
        Percent,
        WrapContent,
        MatchParent
    };

    struct LayoutValue {
        SizeMode mode = SizeMode::WrapContent;
        float value = 0.0f;

        static LayoutValue fixed(float v);
        static LayoutValue percent(float v);
        static LayoutValue wrapContent();
        static LayoutValue matchParent();
    };

    struct LayoutParams {
        LayoutValue width;
        LayoutValue height;

        static LayoutParams fixed(float w, float h);
        static LayoutParams percent(float w, float h);
        static LayoutParams setMode(SizeMode w, SizeMode h);
    };

}

#endif //CUBE_LAYOUTPARAMS_HPP