/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CAMERA_ICAMERA_FOLLOWER_HPP
#define MGC_CAMERA_ICAMERA_FOLLOWER_HPP

#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/math/vec2.hpp"

namespace mgc {
namespace camera {

struct ICameraFollower {

    virtual ~ICameraFollower() = default;

    virtual mgc::math::Vec2i follow_position() const = 0;

};

}// namespace camera
}// namespace mgc

#endif/*MGC_CAMERA_ICAMERA_FOLLOWER_HPP*/

