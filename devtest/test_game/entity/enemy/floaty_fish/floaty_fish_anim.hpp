#ifndef MGC_ENEMY_FLOATY_FISH_ANIM_HPP
#define MGC_ENEMY_FLOATY_FISH_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/floaty_fish/anim_floaty_fish.h"

namespace app {
namespace enemy {

enum class FloatyFishAnimState {
    FloatingRight,
    FloatingLeft,
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(FloatyFishAnimState state) {
    switch( state ) {
    case FloatyFishAnimState::FloatingRight: return anim_floaty_fish_floating_right;
    case FloatyFishAnimState::FloatingLeft: return anim_floaty_fish_floating_left;
    default: return anim_floaty_fish_floating_right;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_FLOATY_FISH_ANIM_HPP*/

