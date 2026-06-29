#include "game_over_effect.hpp"
#include "resources/mml/mml.h"
#include "entity/player/player.hpp"

namespace app {

void GameOverEffect::proc(mgc::graphics::Framebuffer& fb) {
    switch (state_) {
    case GameOverState::NotStarted:
        if ( player_.is_game_over() ) {
            state_ = GameOverState::MusicPlaying;
            sound_controller_.play_background_music(
                MML_BGM_1_THE_FLEA_WALTZ_END
            );
            sw_.start();
        }
        break;
    case GameOverState::MusicPlaying:
        if ( ( sound_controller_.has_background_music_finished() ) ||
             ( sw_.elapsed_ms() >= 5000 )
        ) { 
            state_ = GameOverState::Blackout;
            screen_fader_.request_fade_out();
            sw_.reset();
        }
        break;

    case GameOverState::Blackout:
        
        if ( screen_fader_.fade_state() == ScreenFaderState::FadeOutComplete ) {
            state_ = GameOverState::Finished;
        }
        break;

    case GameOverState::Finished:
        break;
    default:
        break;
    }

    screen_fader_.update(fb);
}

}
