#ifndef MGC_GAME_CONTEXT_HPP
#define MGC_GAME_CONTEXT_HPP

#include "app_common.hpp"
#include "default_talkflow_effects.hpp"
#include "entity/player/player.hpp"
#include "scene_info.hpp"
#include "checkpoint_info.hpp"
#include "status_display_request.hpp"

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
        default_talkflow_effects_(sound_controller),
        player_(player) { }
    GameContext(const GameContext&) = delete;
    GameContext& operator=(const GameContext&) = delete;
    GameContext(GameContext&&) = default;
    GameContext& operator=(GameContext&&) = default;
    ~GameContext() = default;

    Player& player() { return player_; }
    const FrameTimerT& frame_timer() { return frame_timer_; }
    GamepadT& gamepad() { return gamepad_; }
    SoundControllerT& sound_controller() { return sound_controller_; }

    SceneInfo& scene_info() { return scene_info_; }
    CheckpointInfo& checkpoint_info() { return checkpoint_info_; }

    StatusDisplayRequest& status_display_request() { return status_display_request_; }
    DefaultTalkflowEffects& default_talkflow_effects() { return default_talkflow_effects_; }
private:
    DisplayDriverT& display_driver_;
    SoundControllerT& sound_controller_;
    GamepadT& gamepad_;
    FrameTimerT& frame_timer_;
    Player& player_;

    SceneInfo scene_info_;
    CheckpointInfo checkpoint_info_;

    StatusDisplayRequest status_display_request_;
    DefaultTalkflowEffects default_talkflow_effects_;
};

} // namespace app

#endif/*MGC_GAME_CONTEXT_HPP*/

