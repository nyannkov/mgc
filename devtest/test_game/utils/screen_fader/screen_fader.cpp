#include "screen_fader.hpp"

namespace app {


void ScreenFader::clear() {
    state_ = ScreenFaderState::None;
    fade_area_ = 0;
    alpha_ = 0;
    fill_color_ = MGC_COLOR_BLACK;
}

void ScreenFader::request_fade_in(ScreenFaderType type) {
    fade_area_ = 0;
    alpha_ = 0;
    state_ = ScreenFaderState::FadeIn;
    type_ = type;
}

void ScreenFader::request_fade_out(ScreenFaderType type) {
    fade_area_ = 0;
    alpha_ = 255;
    state_ = ScreenFaderState::FadeOut;
    type_ = type;
}

void ScreenFader::update(FramebufferT& fb) {
    uint16_t width = fb.width();
    uint16_t height = fb.height();
    ColorT* buffer = fb.data();

    if ( buffer == nullptr ) { return; }

    if ( fade_area_ > width ) { fade_area_ = width; } 

    if ( state_ == ScreenFaderState::FadeIn ) {

        if ( fade_speed_ == 0 ) {
            state_ = ScreenFaderState::FadeInComplete;
        } else {
            if ( fade_in(fb) ) {
                state_ = ScreenFaderState::FadeInComplete;
            }
        }
    } else if ( state_ == ScreenFaderState::FadeOut ) {

        if ( fade_speed_ == 0 ) {
            fb.clear(fill_color_);
            state_ = ScreenFaderState::FadeOutComplete;
        } else {
            if ( fade_out(fb) ) {
                state_ = ScreenFaderState::FadeOutComplete;
            }
        }

    } else if ( state_ == ScreenFaderState::FadeOutComplete ) {
        
        fb.clear(fill_color_);

    } else { }
}

bool ScreenFader::fade_in(FramebufferT& fb) {
    if ( type_ == ScreenFaderType::WipeLeft ) {
        uint16_t width = fb.width();
        uint16_t height = fb.height();
        for ( uint16_t w = fade_area_; w < width; w++ ) {
            for ( uint16_t h = 0; h < height; h++ ) {
                fb.draw_pixel(w, h, fill_color_);
            }
        }

        if ( fade_area_ == width ) {
            return true;
        } else {
            fade_area_ += fade_speed_;
            return false;
        }
    } else {
        
        fill_color_with_alpha(fb, fill_color_, alpha_);

        if ( alpha_ == 255 ) {
            return true;
        } else {
            if ( ( static_cast<uint16_t>(alpha_) + fade_speed_ ) > 255 ) {
                alpha_ = 255;
            } else {
                alpha_ += fade_speed_;
            }
            return false;
        }
    }
}

bool ScreenFader::fade_out(FramebufferT& fb) {
    if ( type_ == ScreenFaderType::WipeLeft ) {
        uint16_t width = fb.width();
        uint16_t height = fb.height();
        for ( uint16_t w = 0; w < fade_area_; w++ ) {
            for ( uint16_t h = 0; h < height; h++ ) {
                fb.draw_pixel(w, h, fill_color_);
            }
        }

        if ( fade_area_ == width ) {
            return true;
        } else {
            fade_area_ += fade_speed_;
            return false;
        }
    } else {
        fill_color_with_alpha(fb, fill_color_, alpha_);

        if ( alpha_ == 0 ) {
            return true;
        } else {
            if (  static_cast<int16_t>(alpha_) < fade_speed_ ) {
                alpha_ = 0;
            } else {
                alpha_ -= fade_speed_;
            }
            return false;
        }
    }
}

} // namespace app

