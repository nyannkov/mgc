#ifndef MGC_GAME_CONTEXT_HPP
#define MGC_GAME_CONTEXT_HPP

#include "app_common.hpp"
#include "entity/player/player.hpp"
#include "game_context/status_display_request.hpp"
#include "game_context/stage_info.hpp"
#include "game_context/player_stats.hpp"

namespace app {

struct GameContext {
    GameContext(
        DisplayDriverT& display_driver,
        SoundControllerT& sound_controller,
        GamepadT& gamepad,
        FrameTimerT& frame_timer,
        Player& player
    ) : display_driver_(display_driver),
        sound_controller_(sound_controller),
        gamepad_(gamepad),
        frame_timer_(frame_timer),
        player_(player) { }
    ~GameContext() = default;
    GameContext(const GameContext&) = delete;
    GameContext& operator=(const GameContext&) = delete;
    GameContext(GameContext&&) = default;
    GameContext& operator=(GameContext&&) = default;

    Player& player() { return player_; }
    const FrameTimerT& frame_timer() { return frame_timer_; }
    GamepadT& gamepad() { return gamepad_; }
    SoundControllerT& sound_controller() { return sound_controller_; }
    StatusDisplayRequest& status_display_request() { return status_display_request_; }
    StageInfo& stage_info() { return stage_info_; }
    PlayerStats& player_stats() { return player_stats_; }

private:
    DisplayDriverT& display_driver_;
    SoundControllerT& sound_controller_;
    GamepadT& gamepad_;
    FrameTimerT& frame_timer_;
    Player& player_;
    StatusDisplayRequest status_display_request_{};
    StageInfo stage_info_{};
    PlayerStats player_stats_{};
};

} // namespace app

#endif/*MGC_GAME_CONTEXT_HPP*/

