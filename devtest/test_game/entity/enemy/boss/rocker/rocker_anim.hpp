#ifndef MGC_ENEMY_ROCKER_ANIM_HPP
#define MGC_ENEMY_ROCKER_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/rocker/anim_boss_rocker.h"

namespace app {
namespace enemy {

enum class RockerAnimState {
    Stand,
    Stand2,
    Playing,
    WidePlaying,
    Singing,
    Stepping,
    Waving,
    StopWaving
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(RockerAnimState state) {
    switch( state ) {
    case RockerAnimState::Stand: return anim_boss_rocker_stand;
    case RockerAnimState::Stand2: return anim_boss_rocker_stand2;
    case RockerAnimState::Playing: return anim_boss_rocker_playing;
    case RockerAnimState::WidePlaying: return anim_boss_rocker_wide_playing;
    case RockerAnimState::Singing: return anim_boss_rocker_singing;
    case RockerAnimState::Stepping: return anim_boss_rocker_stepping;
    case RockerAnimState::Waving: return anim_boss_rocker_waving;
    case RockerAnimState::StopWaving: return anim_boss_rocker_stop_waving;
    default: return anim_boss_rocker_stand;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_ROCKER_ANIM_HPP*/

