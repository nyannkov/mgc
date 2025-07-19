#ifndef MGC_GAME_CONTEXT_HPP
#define MGC_GAME_CONTEXT_HPP

#include "mgc_cpp/mgc.hpp"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"

namespace app {

struct Player;

struct GameContext {
    using IButtonT = mgc::platform::input::IButton;
    using ISoundT = mgc::platform::sound::ISoundController;
    using TimerT = mgc::drivers::platform::timer::FreeRunningTimerU32;
    using TimerValueT = TimerT::timestamp_t;

    GameContext(const IButtonT& button, ISoundT& sound, const Player& player)
        : button_(button),
          sound_(sound),
          player_(player) { }
          
    TimerValueT timer_value_ms() {
        return TimerT::now_ms();
    }

    const IButtonT& button() const {
        return button_;
    }

    ISoundT& sound() {
        return sound_;
    }

    bool is_any_key_pressed() const {
        return mgc::platform::input::is_any_button_pressed(button_);
    }

    const Player& player() const {
        return player_;
    }

private:
    const IButtonT& button_;
    ISoundT& sound_;
    const Player& player_;
};


}// namespace app

#endif/*MGC_GAME_CONTEXT_HPP*/

