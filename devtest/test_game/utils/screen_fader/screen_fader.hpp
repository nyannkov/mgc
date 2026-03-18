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

enum class ScreenFaderType {
    WipeLeft,
    Fade,
};


//TODO refactor
struct ScreenFader {

    ScreenFader() : fade_speed_(8) {
        clear();
    }
    ~ScreenFader() = default;

    void clear() {
        state_ = ScreenFaderState::None;
        fade_area_ = 0;
        alpha_ = 0;
        fill_color_ = MGC_COLOR_BLACK;
    }

    void set_fill_color(mgc::graphics::Color color) {
        fill_color_ = color;
    }
    mgc::graphics::Color fill_color() const { return fill_color_; }

    void request_fade_in(ScreenFaderType type = ScreenFaderType::WipeLeft) {
        fade_area_ = 0;
        alpha_ = 0;
        state_ = ScreenFaderState::FadeIn;
        type_ = type;
    }

    void request_fade_out(ScreenFaderType type = ScreenFaderType::WipeLeft) {
        fade_area_ = 0;
        alpha_ = 255;
        state_ = ScreenFaderState::FadeOut;
        type_ = type;
    }

    void update(mgc::graphics::Framebuffer& fb) {
        uint16_t width = fb.width();
        uint16_t height = fb.height();
        mgc::graphics::Color* buffer = fb.data();

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

    void set_fade_speed(uint16_t speed) {
        fade_speed_ = speed;
    }

    uint16_t fade_speed() const {
        return fade_speed_;
    }

    ScreenFaderState fade_state() const {
        return state_;
    }

private:
    ScreenFaderState state_;
    ScreenFaderType type_;
    int32_t fade_area_;
    uint16_t fade_speed_;
    bool is_fade_out_;
    mgc::graphics::Color fill_color_;
    uint8_t alpha_;

    bool fade_in(FramebufferT& fb) {
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
            
            draw_pixel(fb, fill_color_, alpha_);

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

    bool fade_out(FramebufferT& fb) {
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
            draw_pixel(fb, fill_color_, alpha_);

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

    //TODO library
    static void draw_pixel(FramebufferT& fb, mgc::graphics::Color fill_color, uint8_t alpha) {
        uint16_t width = fb.width();
        uint16_t height = fb.height();
        const mgc::graphics::Color* draw_buf = fb.data();
        mgc::graphics::Color dst = fill_color;

        for ( uint16_t x = 0; x < width; x++ ) {
            for ( uint16_t y = 0; y < height; y++ ) {
                mgc::graphics::Color src = MGC_COLOR_SWAP(draw_buf[x + width*y]);
                uint8_t sr = (src >> 11) & 0x1F;
                uint8_t sg = (src >>  5) & 0x3F;
                uint8_t sb =  src        & 0x1F;
                uint8_t dr = (dst >> 11) & 0x1F;
                uint8_t dg = (dst >>  5) & 0x3F;
                uint8_t db =  dst        & 0x1F;
                uint8_t r = (sr * alpha + dr * (255 - alpha) + 128) >> 8;
                uint8_t g = (sg * alpha + dg * (255 - alpha) + 128) >> 8;
                uint8_t b = (sb * alpha + db * (255 - alpha) + 128) >> 8;
                src = ((mgc::graphics::Color)r<<11)|((mgc::graphics::Color)g<<5)|((mgc::graphics::Color)b<<0);
                fb.draw_pixel(x, y, src);
            }
        }
    }
};

}// namespace app

#endif/*MGC_UTILS_SCREEN_FADER_HPP*/

