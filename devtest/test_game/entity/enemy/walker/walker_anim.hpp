#ifndef MGC_ENEMY_WALKER_ANIM_HPP
#define MGC_ENEMY_WALKER_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/walker/anim_walker.h"

namespace app {
namespace enemy {

enum class WalkerAnimState {
    StandRight,
    StandLeft,
    WalkRight,
    WalkLeft,
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(WalkerAnimState state) {
    switch( state ) {
    case WalkerAnimState::StandRight: return anim_walker_stand_right;
    case WalkerAnimState::StandLeft: return anim_walker_stand_left;
    case WalkerAnimState::WalkRight: return anim_walker_walk_right;
    case WalkerAnimState::WalkLeft: return anim_walker_walk_left;
    default: return anim_walker_stand_right;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_WALKER_ANIM_HPP*/

