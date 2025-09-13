#ifndef MGC_UTILS_SCREEN_FADER_HPP
#define MGC_UTILS_SCREEN_FADER_HPP

#include "mgc_cpp/mgc.hpp"

namespace app {

enum class ScreenFaderState {
    None,
    FadeIn,
    FadeInComplete,
    FadeOut,
    FadeOutComplete,
};

struct ScreenFader {

    ScreenFader() : fade_speed_(8) {
        clear();
    }
    ~ScreenFader() = default;

    void clear() {
        state_ = ScreenFaderState::None;
        fade_area_ = 0;
        is_fade_out_ = false;
    }

    void request_fade_in(const mgc::graphics::Framebuffer& fb) {
        fade_area_ = 0;
        state_ = ScreenFaderState::FadeIn;
        is_fade_out_ = false;
    }

    void request_fade_out(const mgc::graphics::Framebuffer& fb) {
        fade_area_ = 0;
        state_ = ScreenFaderState::FadeOut;
        is_fade_out_ = true;
    }

    void update(mgc::graphics::Framebuffer& fb) {
        uint16_t width = fb.width();
        uint16_t height = fb.height();
        mgc::graphics::Color* buffer = fb.data();

        if ( buffer == nullptr ) { return; }

        if ( fade_area_ > width ) { fade_area_ = width; } 

        if ( state_ == ScreenFaderState::FadeIn ) {

            for ( uint16_t w = fade_area_; w < width; w++ ) {
                for ( uint16_t h = 0; h < height; h++ ) {
                    fb.draw_pixel(w, h, MGC_COLOR_BLACK);
                }
            }

            if ( fade_area_ == width ) {
                state_ = ScreenFaderState::FadeInComplete;
            } else {
                fade_area_ += fade_speed_;
            }

        } else if ( state_ == ScreenFaderState::FadeOut ) {

            for ( uint16_t w = 0; w < fade_area_; w++ ) {
                for ( uint16_t h = 0; h < height; h++ ) {
                    fb.draw_pixel(w, h, MGC_COLOR_BLACK);
                }
            }

            if ( fade_area_ == width ) {
                state_ = ScreenFaderState::FadeOutComplete;
            } else {
                fade_area_ += fade_speed_;
            }

        } else if ( state_ == ScreenFaderState::FadeOutComplete ) {
            
            if ( is_fade_out_ ) {
                fb.clear( MGC_COLOR_BLACK);
            }

        } else { }
    }

    void set_fade_speed(uint16_t speed) {
        fade_speed_ = speed;
    }

    ScreenFaderState fade_state() const {
        return state_;
    }

private:
    ScreenFaderState state_;
    int32_t fade_area_;
    uint16_t fade_speed_;
    bool is_fade_out_;
};

}// namespace app

#endif/*MGC_UTILS_SCREEN_FADER_HPP*/

