#include "scene_999_game_over.hpp"
#include "resources/mml/mml.h"

namespace app {

using mgc::platform::input::Key;

Scene999_GameOver::Scene999_GameOver(GameContext& ctx)
                : SceneBase(ctx),
                  player_(ctx.player()) {
      id_ = SceneId::Id_999_GameOver;
      id_next_ = SceneId::Id_999_GameOver;
}

void Scene999_GameOver::init() {

    talkflow_.set_talkscript(talkscript_1);
    talkflow_.bind_listener(talkflow_listener_);
    talkflow_.begin(MGC_TALKSCRIPT_1_GAME_OVER);

    player_.set_position({MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(10)});

    status_display_request_.request_hide();
}

void Scene999_GameOver::update() {

    talkflow_.proc();
    auto request = talkflow_listener_.game_request();
    if ( request != TalkflowListener::GameRequest::None ) {
        if ( request == TalkflowListener::GameRequest::Continue ) {
            player_.set_power_pose_in_game_over();
            id_next_ = id_prev_;
        } else {
            player_.set_void_pose_in_game_over();
            id_next_ = SceneId::Id_000;
        }
        talkflow_listener_.clear_game_request();
    }

    player_.update_animation(false);

    if ( talkflow_.has_finished() ) {
        talkflow_.reset_state();
        scene_change_request_ = true;
        player_.revive();
    }
}

void Scene999_GameOver::draw(mgc::graphics::Framebuffer& fb) {
    fb.clear(MGC_COLOR_BLACK);
    talkflow_.draw(fb);
    player_.draw(fb);
}

} // namespace app

