#ifndef MGC_ENEMY_ANIM_HPP
#define MGC_ENEMY_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/skyfish/anim_skyfish.h"

namespace app {
namespace enemy {

enum class SkyFishAnimState {
    HoverLeft,
    HoverRight,
    RiseLeft,
    RiseRight,
    FallLeft,
    FallRight,
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(SkyFishAnimState state) {
    switch( state ) {
    case SkyFishAnimState::HoverLeft: return anim_skyfish_hover_left;
    case SkyFishAnimState::HoverRight: return anim_skyfish_hover_right;
    case SkyFishAnimState::RiseLeft: return anim_skyfish_rise_left;
    case SkyFishAnimState::RiseRight: return anim_skyfish_rise_right;
    case SkyFishAnimState::FallLeft: return anim_skyfish_fall_left;
    case SkyFishAnimState::FallRight: return anim_skyfish_fall_right;
    default: return anim_skyfish_hover_left;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_ANIM_HPP*/

