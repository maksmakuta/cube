#include "LayoutParams.hpp"

namespace cube {

    LayoutValue LayoutValue::fixed(const float v){
        return {SizeMode::Fixed, v};
    }

    LayoutValue LayoutValue::percent(const float v){
        return {SizeMode::Percent, v};
    }

    LayoutValue LayoutValue::wrapContent(){
        return {SizeMode::WrapContent, 0.f};
    }

    LayoutValue LayoutValue::matchParent(){
        return {SizeMode::MatchParent, 0.f};
    }

    LayoutParams LayoutParams::fixed(const float w, const float h){
        return {
            {SizeMode::Fixed, w},
            {SizeMode::Fixed, h},
        };
    }

    LayoutParams LayoutParams::percent(const float w, const float h){
        return {
            {SizeMode::Percent, w},
            {SizeMode::Percent, h},
        };
    }

    LayoutParams LayoutParams::setMode(const SizeMode w, const SizeMode h){
        return {
            {w, 0.f},
            {h, 0.f},
        };
    }

}