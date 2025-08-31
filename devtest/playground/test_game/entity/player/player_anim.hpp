#ifndef MGC_PLAYER_ANIM_HPP
#define MGC_PLAYER_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/player/anim_player.h"

namespace app {

enum class PlayerAnimState {
    StandLeft,
    StandRight,
    WalkLeft,
    WalkRight,
    JumpLeft,
    JumpRight
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(PlayerAnimState state) {
    switch( state ) {
    case PlayerAnimState::StandLeft: return anim_player_stand_left;
    case PlayerAnimState::StandRight: return anim_player_stand_right;
    case PlayerAnimState::WalkLeft: return anim_player_walk_left;
    case PlayerAnimState::WalkRight: return anim_player_walk_right;
    case PlayerAnimState::JumpLeft: return anim_player_jump_left;
    case PlayerAnimState::JumpRight: return anim_player_jump_right;
    default: return anim_player_stand_right;
    }
}

}// namespace app


#endif/*MGC_PLAYER_ANIM_HPP*/

