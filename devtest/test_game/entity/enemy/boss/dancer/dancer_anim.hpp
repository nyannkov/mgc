#ifndef MGC_ENEMY_DANCER_ANIM_HPP
#define MGC_ENEMY_DANCER_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/dancer/anim_boss_dancer.h"

namespace app {
namespace enemy {

enum class DancerAnimState {
    StaticYeah,
    Yeah,
    WalkLeft,
    WalkRight,
    Boxing
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(DancerAnimState state) {
    switch( state ) {
    case DancerAnimState::StaticYeah: return anim_boss_dancer_static_yeah;
    case DancerAnimState::Yeah: return anim_boss_dancer_yeah;
    case DancerAnimState::WalkLeft: return anim_boss_dancer_walk_left;
    case DancerAnimState::WalkRight: return anim_boss_dancer_walk_right;
    case DancerAnimState::Boxing: return anim_boss_dancer_boxing;
    default: return anim_boss_dancer_yeah;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_DANCER_ANIM_HPP*/

