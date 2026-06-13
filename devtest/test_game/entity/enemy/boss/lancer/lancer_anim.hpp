#ifndef MGC_ENEMY_LANCER_ANIM_HPP
#define MGC_ENEMY_LANCER_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/lancer/anim_boss_lancer.h"

namespace app {
namespace enemy {

enum class LancerAnimState {
    Stand,
    WalkLeft,
    WalkRight,
    ThrowLeft,
    ThrowRight,
    ThrowUp,
    SeeYouNext,
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(LancerAnimState state) {
    switch( state ) {
    case LancerAnimState::Stand: return anim_boss_lancer_stand;
    case LancerAnimState::WalkLeft: return anim_boss_lancer_walk_left;
    case LancerAnimState::WalkRight: return anim_boss_lancer_walk_right;
    case LancerAnimState::ThrowLeft: return anim_boss_lancer_throw_left;
    case LancerAnimState::ThrowRight: return anim_boss_lancer_throw_right;
    case LancerAnimState::ThrowUp: return anim_boss_lancer_throw_up;
    case LancerAnimState::SeeYouNext: return anim_boss_lancer_see_you_next;
    default: return anim_boss_lancer_walk_left;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_LANCER_ANIM_HPP*/

