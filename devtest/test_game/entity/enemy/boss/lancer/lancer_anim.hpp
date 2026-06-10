#ifndef MGC_ENEMY_LANCER_ANIM_HPP
#define MGC_ENEMY_LANCER_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/lancer/anim_boss_lancer.h"

namespace app {
namespace enemy {

enum class LancerAnimState {
    WalkLeft,
    WalkRight,
    ThrowLeft,
    ThrowRight,
    ThrowUp
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(LancerAnimState state) {
    switch( state ) {
    case LancerAnimState::WalkLeft: return anim_boss_lancer_walk_left;
    case LancerAnimState::WalkRight: return anim_boss_lancer_walk_right;
    case LancerAnimState::ThrowLeft: return anim_boss_lancer_throw_left;
    case LancerAnimState::ThrowRight: return anim_boss_lancer_throw_right;
    case LancerAnimState::ThrowUp: return anim_boss_lancer_throw_up;
    default: return anim_boss_lancer_walk_left;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_LANCER_ANIM_HPP*/

