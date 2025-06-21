/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CAMERA_SIMPLE_CAMERA_FOLLOWER_HPP
#define MGC_CAMERA_SIMPLE_CAMERA_FOLLOWER_HPP

#include "mgc/render/camera.h"
#include "mgc_cpp/math/vec2.hpp"
#include "mgc_cpp/features/has_position.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/internal/common.hpp"
#include "icamera_follower.hpp"

namespace mgc {
namespace camera {

struct SimpleCameraFollower : mgc::camera::ICameraFollower,
                              mgc::features::Resettable {

    SimpleCameraFollower() { reset(); }
    ~SimpleCameraFollower() = default;

    SimpleCameraFollower(const SimpleCameraFollower&) = delete;
    SimpleCameraFollower& operator=(const SimpleCameraFollower&) = delete;
    SimpleCameraFollower(SimpleCameraFollower&&) = default;
    SimpleCameraFollower& operator=(SimpleCameraFollower&&) = default;

    void set_target(const mgc::features::HasPosition<mgc::math::Vec2i>& target) {
        target_ = &target;
    }

    void set_x_follow_setting(int16_t start_line, int16_t end_line, uint16_t deadzone) {
        camera_set_x_follow_settings(&camera_, start_line, end_line, deadzone);
    }

    void set_y_follow_setting(int16_t start_line, int16_t end_line, uint16_t deadzone) {
        camera_set_y_follow_settings(&camera_, start_line, end_line, deadzone);
    }

    void set_x_follow_enabled(bool enabled) {
        camera_set_x_follow_enabled(&camera_, enabled);
    }

    void set_y_follow_enabled(bool enabled) {
        camera_set_y_follow_enabled(&camera_, enabled);
    }

    void update_follow_position() {
        if ( target_ ) {
            auto position = target_->position();
            camera_follow_target_position(&camera_, mgc_point_t{position.x, position.y});
        }
    }

    mgc::math::Vec2i follow_position() const override {
        mgc_point_t follow_point;
        if ( camera_get_position(&camera_, &follow_point) ) {
            return mgc::math::Vec2i(follow_point.x, follow_point.y);
        } else {
            return mgc::math::Vec2i(0, 0);
        }
    }

    // [features] Resettable
    void reset() {
        camera_init(&camera_);
    }


private:
    mgc_camera_t camera_;
    const mgc::features::HasPosition<mgc::math::Vec2i>* target_;
};

}// namespace camera
}// namespace mgc

#endif/*MGC_CAMERA_SIMPLE_CAMERA_FOLLOWER_HPP*/
