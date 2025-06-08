/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CAMERA_SIMPLE_CAMERA_FOLLOWER_HPP
#define MGC_CAMERA_SIMPLE_CAMERA_FOLLOWER_HPP

#include "mgc/render/camera.h"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/geometry/point.hpp"
#include "icamera_follower.hpp"

namespace mgc {
namespace camera {

struct SimpleCameraFollower : mgc::camera::ICameraFollower {

    SimpleCameraFollower() {
        reset(); 
    }

    ~SimpleCameraFollower() = default;

    void reset() {
        camera_init(&camera_);
    }

    void set_target(const mgc::features::Positionable& target) {
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
            camera_follow_target_position(&camera_, target_->get_position().to_c());
        }
    }

    mgc::geometry::Point get_follow_position() const override {
        mgc_point_t follow_point;
        if ( camera_get_position(&camera_, &follow_point) ) {
            return mgc::geometry::Point::from_c(follow_point);
        } else {
            return mgc::geometry::Point(0, 0);
        }
    }

private:
    mgc_camera_t camera_;
    const mgc::features::Positionable* target_;
};

}// namespace camera
}// namespace mgc

#endif/*MGC_CAMERA_SIMPLE_CAMERA_FOLLOWER_HPP*/
