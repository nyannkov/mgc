#ifndef MGC_UTILS_SCREEN_FADER_HPP
#define MGC_UTILS_SCREEN_FADER_HPP

#include "app_common.hpp"

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

    void clear();

    void set_fill_color(ColorT color) { fill_color_ = color; }
    ColorT fill_color() const { return fill_color_; }

    void request_fade_in(ScreenFaderType type = ScreenFaderType::WipeLeft);
    void request_fade_out(ScreenFaderType type = ScreenFaderType::WipeLeft);

    void update(FramebufferT& fb);
    void set_fade_speed(uint16_t speed) { fade_speed_ = speed; }
    uint16_t fade_speed() const { return fade_speed_; }
    ScreenFaderState fade_state() const { return state_; }

private:
    ScreenFaderState state_;
    ScreenFaderType type_;
    int32_t fade_area_;
    uint16_t fade_speed_;
    bool is_fade_out_;
    ColorT fill_color_;
    uint8_t alpha_;

    bool fade_in(FramebufferT& fb); 
    bool fade_out(FramebufferT& fb); 

    //TODO library
    static void draw_pixel(FramebufferT& fb, ColorT fill_color, uint8_t alpha); 
};

}// namespace app

#endif/*MGC_UTILS_SCREEN_FADER_HPP*/

