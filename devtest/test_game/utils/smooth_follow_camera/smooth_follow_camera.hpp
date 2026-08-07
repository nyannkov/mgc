#ifndef MGC_UTILS_SMOOTH_FOLLOW_CAMERA_HPP
#define MGC_UTILS_SMOOTH_FOLLOW_CAMERA_HPP

#include "app_common.hpp"

namespace app {

struct SmoothFollowCamera : mgc::camera::ICameraFollower {

    void set_target(const mgc::features::HasPosition<mgc::math::Vec2i>& target) {
        target_ = &target;
    }

    void set_target_position_in_camera_space(mgc::math::Vec2i target_pos_in_camera) {

        target_pos_in_camera_ = target_pos_in_camera;
    }

    void set_x_follow_setting(mgc_world_t min_x, mgc_world_t max_x) {
        min_x_ = min_x;
        max_x_ = max_x;
    }

    void set_y_follow_setting(mgc_world_t min_y, mgc_world_t max_y) {
        min_y_ = min_y;
        max_y_ = max_y;
    }

    void set_x_follow_enabled(bool enabled) {
        x_follow_enabled_ = enabled;
    }

    void set_y_follow_enabled(bool enabled) {
        y_follow_enabled_ = enabled;
    }

    void update_follow_position() override {
        if( target_ ) {

            auto pos = target_->position();

            if ( x_follow_enabled_ ) {
                if ( pos.x < min_x_ ) {
                    pos.x = min_x_;
                } else if ( max_x_ < pos.x ) {
                    pos.x = max_x_;
                } else { }

                float dx = 0.1* (pos.x - (float)target_pos_in_camera_.x - posf_.x);
                posf_.x += dx;
            }

            if ( y_follow_enabled_ ) {
                if ( pos.y < min_y_ ) {
                    pos.y = min_y_;
                } else if ( max_y_ < pos.y ) {
                    pos.y = max_y_;
                } else { }

                float dy = 0.1* (pos.y - (float)target_pos_in_camera_.y - posf_.y);
                posf_.y += dy;
            }

        } else {
            posf_ = { 0, 0 };
        }
    }
    
    void snap_to_target() {

        auto pos = target_->position();

        if ( x_follow_enabled_ ) {
            if ( pos.x < min_x_ ) {
                pos.x = min_x_;
            } else if ( max_x_ < pos.x ) {
                pos.x = max_x_;
            } else { }
            posf_.x = pos.x - target_pos_in_camera_.x; 
        }

        if ( y_follow_enabled_ ) {
            if ( pos.y < min_y_ ) {
                pos.y = min_y_;
            } else if ( max_y_ < pos.y ) {
                pos.y = max_y_;
            } else { }
            posf_.y = pos.y - target_pos_in_camera_.y; 
        }
    }

    mgc::math::Vec2i follow_position() const override {
        if( target_ ) {
            return { (int16_t)posf_.x, (int16_t)posf_.y };
        } else {
            return mgc::math::Vec2i(0, 0);
        }
    }

private:
    mgc::math::Vec2f posf_ = {0, 0};
    mgc::math::Vec2i target_pos_in_camera_ = {0, 0};
    mgc_world_t min_x_ = 0;
    mgc_world_t max_x_ = 0;
    mgc_world_t min_y_ = 0;
    mgc_world_t max_y_ = 0;
    bool x_follow_enabled_ = false;
    bool y_follow_enabled_ = false;

    const mgc::features::HasPosition<mgc::math::Vec2i>* target_ = nullptr;
};


}// namespace app

#endif/*MGC_UTILS_SMOOTH_FOLLOW_CAMERA_HPP*/

