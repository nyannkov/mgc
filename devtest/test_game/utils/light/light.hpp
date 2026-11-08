#ifndef MGC_LIGHT_HPP
#define MGC_LIGHT_HPP

#include <math.h>

namespace app {

struct Light {
    Light() {
        pos_ = { 0, 0 };
        radius_ = 1.f;
        color_ = MGC_COLOR_BLACK;
    }

    void set_raduis(float r) { radius_ = r; }
    auto radius() const { return radius_; }

    void set_position(mgc::math::Vec2i p) { pos_ = p; }
    auto position() const { return pos_; }

    bool draw(mgc::graphics::Framebuffer &fb, mgc::math::Vec2i& cam_pos) const {
        
        if ( radius_ == 0 ) {
            return false;
        }

        mgc::math::Vec2i pos_in_screen = pos_ - cam_pos;

        float min_circle_x = pos_in_screen.x - radius_;
        float max_circle_x = pos_in_screen.x + radius_;
        float min_circle_y = pos_in_screen.y - radius_;
        float max_circle_y = pos_in_screen.y + radius_;
        int32_t x, y;
        uint16_t width, height;

        width = fb.width();
        height = fb.height();

        for ( y = 0; y < min_circle_y; y++ ) {
            for ( x = 0; x < width; x++ ) {
               fb.draw_pixel(x, y, MGC_COLOR_BLACK);
            }
        }

        for ( y = min_circle_y; y < max_circle_y; y++ ) {
            float x_l, x_r;
            float r_r = radius_*radius_;
            float y_y = (y - pos_in_screen.y) * (y - pos_in_screen.y);
            x_l = pos_in_screen.x - sqrt(r_r - y_y);
            x_r = pos_in_screen.x + sqrt(r_r - y_y);

            for ( x = 0; x < min_circle_x; x++ ) {
                fb.draw_pixel(x, y, MGC_COLOR_BLACK);
            }

            for ( x = min_circle_x; x < x_l; x++ ) {
                fb.draw_pixel(x, y, MGC_COLOR_BLACK);
            }

            for ( x = x_r; x < max_circle_x; x++ ) {
                fb.draw_pixel(x, y, MGC_COLOR_BLACK);
            }

            for ( x = max_circle_x; x < width; x++ ) {
                fb.draw_pixel(x, y, MGC_COLOR_BLACK);
            }
        }
    
        for ( y = max_circle_y; y < height; y++ ) {
            for ( x = 0; x < width; x++ ) {
               fb.draw_pixel(x, y, MGC_COLOR_BLACK);
            }
        }

        return true;
    }

private:
    mgc::math::Vec2i pos_;
    float radius_;
    ColorT color_;
};

}// namespace app

#endif/*MGC_LIGHT_HPP*/

