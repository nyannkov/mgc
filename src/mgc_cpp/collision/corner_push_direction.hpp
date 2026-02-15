/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2026 nyannkov
 */
#ifndef MGC_COLLISION_CORNER_PUSH_DIRECTION_HPP
#define MGC_COLLISION_CORNER_PUSH_DIRECTION_HPP

#include "mgc/detector/collision_common.h"

namespace mgc {
namespace collision {

enum class CornerPushDirection {
    PreferX,
    PreferY,
    None
};

inline enum mgc_corner_push_dir to_c_dir(CornerPushDirection push_dir) {
    switch ( push_dir ) {
    case CornerPushDirection::PreferX: return MGC_CORNER_PUSH_DIR_PREFER_X;
    case CornerPushDirection::PreferY: return MGC_CORNER_PUSH_DIR_PREFER_Y;
    default: return MGC_CORNER_PUSH_DIR_NONE;
    }
}

}// namespace collision
}// namespace mgc

#endif/*MGC_COLLISION_CORNER_PUSH_DIRECTION_HPP*/
