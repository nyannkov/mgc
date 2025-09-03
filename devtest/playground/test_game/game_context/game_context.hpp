#ifndef MGC_GAME_CONTEXT_HPP
#define MGC_GAME_CONTEXT_HPP

#include "app_common.hpp"
#include "entity/player/player.hpp"

namespace app {

struct GameContext {
    GameContext(
        DisplayDriverT& display_driver,
        SoundControllerT& sound_controller,
        GamepadT& gamepad,
        FrameTimerT& frame_timer
    ) : display_driver_(display_driver),
        sound_controller_(sound_controller),
        gamepad_(gamepad),
        frame_timer_(frame_timer),
        player_(frame_timer, gamepad) { }
    ~GameContext() = default;
    GameContext(const GameContext&) = delete;
    GameContext& operator=(const GameContext&) = delete;
    GameContext(GameContext&&) = default;
    GameContext& operator=(GameContext&&) = default;

    Player& player() { return player_; }

    const FrameTimerT& frame_timer() { return frame_timer_; }
    const GamepadT& gamepad() { return gamepad_; }

private:
    DisplayDriverT& display_driver_;
    SoundControllerT& sound_controller_;
    GamepadT& gamepad_;
    FrameTimerT& frame_timer_;
    Player player_;
};

} // namespace app

#endif/*MGC_GAME_CONTEXT_HPP*/

