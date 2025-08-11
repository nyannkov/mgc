#ifndef MGC_GAME_CONTEXT_HPP
#define MGC_GAME_CONTEXT_HPP

#include "mgc_cpp/mgc.hpp"

namespace app {

struct GameContext {
    using IButtonT = mgc::platform::input::IButton;
    using ISoundT = mgc::platform::sound::ISoundController;

    GameContext(const IButtonT& button, ISoundT& sound, mgc::math::Vec2i player_pos, const size_t& player_life)
        : button_(button),
          sound_(sound),
          player_pos_(player_pos),
          player_life_(player_life) { }
          
    const IButtonT& button() const {
        return button_;
    }

    ISoundT& sound() {
        return sound_;
    }

    bool is_any_key_pressed() const {
        return mgc::platform::input::is_any_button_pressed(button_);
    }

    const mgc::math::Vec2i& player_position() const {
        return player_pos_;
    }

    size_t player_life() const {
        return player_life_;
    }

private:
    const IButtonT& button_;
    ISoundT& sound_;
    mgc::math::Vec2i& player_pos_;
    const size_t& player_life_;
};


}// namespace app

#endif/*MGC_GAME_CONTEXT_HPP*/

