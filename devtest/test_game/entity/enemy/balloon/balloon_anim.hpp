#ifndef MGC_ENEMY_BALLOON_ANIM_HPP
#define MGC_ENEMY_BALLOON_ANIM_HPP

#include "mgc_cpp/mgc.hpp"
#include "resources/generated/anim/enemy/balloon/anim_balloon.h"

namespace app {
namespace enemy {

enum class BalloonAnimState {
    FlutterRight,
    FlutterLeft,
    FallRight,
    FallLeft,
};

inline const mgc::parts::assets::AnimFrames& get_anim_frames(BalloonAnimState state) {
    switch( state ) {
    case BalloonAnimState::FlutterRight: return anim_balloon_flutter_right;
    case BalloonAnimState::FlutterLeft: return anim_balloon_flutter_left;
    case BalloonAnimState::FallRight: return anim_balloon_fall_right;
    case BalloonAnimState::FallLeft: return anim_balloon_fall_left;
    default: return anim_balloon_fall_right;
    }
}

}// namespace enemy
}// namespace app


#endif/*MGC_ENEMY_BALLOON_ANIM_HPP*/

