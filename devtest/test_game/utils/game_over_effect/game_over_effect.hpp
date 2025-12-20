#ifndef MGC_GAME_OVER_EFFECT_HPP
#define MGC_GAME_OVER_EFFECT_HPP

#include "app_common.hpp"
#include "utils/screen_fader/screen_fader.hpp"

namespace app {

struct GameOverEffect {
    GameOverEffect(
        const Player& player,
        const FrameTimerT& frame_timer,
        SoundControllerT& sound_controller
    ) : player_(player),
        sw_(frame_timer),
        sound_controller_(sound_controller) {

        reset();
    }

    void reset() {
        state_ = GameOverState::NotStarted;
        sw_.reset();
        screen_fader_.clear();
    }

    void proc(mgc::graphics::Framebuffer& fb);

    bool in_progress() const {
        return (
            ( state_ == GameOverState::MusicPlaying ) ||
            ( state_ == GameOverState::Blackout )
        );
    }

    bool has_finished() const {
        return state_ == GameOverState::Finished;
    }

private:
    enum class GameOverState {
        NotStarted,
        MusicPlaying,
        Blackout,
        Finished
    } state_;
    const Player& player_;
    ScreenFader screen_fader_;
    StopwatchT sw_;
    SoundControllerT& sound_controller_;
};

}// namespace app

#endif// MGC_GAME_OVER_EFFECT_HPP

