#ifndef MGC_ENEMY_SPECTATOR_ANIM_HPP
#define MGC_ENEMY_SPECTATOR_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/spectator/anim_spectator.h"

namespace app {
namespace enemy {

enum class SpectatorAnimState {
    Type1_StandRight,
    Type1_StandLeft,
    Type1_JumpRight,
    Type1_JumpLeft,
    Type2_StandRight,
    Type2_StandLeft,
    Type2_JumpRight,
    Type2_JumpLeft,
    Type3_StandRight,
    Type3_StandLeft,
    Type3_JumpRight,
    Type3_JumpLeft,
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(SpectatorAnimState state) {
    switch( state ) {
    case SpectatorAnimState::Type1_StandRight:  return anim_spectator_type1_stand_right;
    case SpectatorAnimState::Type1_StandLeft:   return anim_spectator_type1_stand_left;
    case SpectatorAnimState::Type1_JumpRight:   return anim_spectator_type1_jump_right;
    case SpectatorAnimState::Type1_JumpLeft:    return anim_spectator_type1_jump_left;
    case SpectatorAnimState::Type2_StandRight:  return anim_spectator_type2_stand_right;
    case SpectatorAnimState::Type2_StandLeft:   return anim_spectator_type2_stand_left;
    case SpectatorAnimState::Type2_JumpRight:   return anim_spectator_type2_jump_right;
    case SpectatorAnimState::Type2_JumpLeft:    return anim_spectator_type2_jump_left;
    case SpectatorAnimState::Type3_StandRight:  return anim_spectator_type3_stand_right;
    case SpectatorAnimState::Type3_StandLeft:   return anim_spectator_type3_stand_left;
    case SpectatorAnimState::Type3_JumpRight:   return anim_spectator_type3_jump_right;
    case SpectatorAnimState::Type3_JumpLeft:    return anim_spectator_type3_jump_left;
    default: return anim_spectator_type1_stand_right;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_SPECTATOR_ANIM_HPP*/

