#include "scene_game_over.hpp"
#include "resources/mml/mml.h"

namespace app {

using mgc::platform::input::Key;

Scene_GameOver::Scene_GameOver(GameContext& ctx)
                : SceneBase(ctx),
                  player_(ctx.world_state.player) {
      set_id(SceneId::GameOver);
}

void Scene_GameOver::init() {

    talkflow_.set_talkscript(talkscript_1);
    talkflow_.bind_listener(talkflow_listener_);
    talkflow_.begin(MGC_TALKSCRIPT_1_GAME_OVER);

    player_.set_position({MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(10)});

    status_display_request_.request_hide();
}

void Scene_GameOver::update() {

    talkflow_.proc();
    auto request = talkflow_listener_.game_request();
    if ( request != TalkflowListener::GameRequest::None ) {
        if ( request == TalkflowListener::GameRequest::Continue ) {
            player_.set_power_pose_in_game_over();
            id_next_ = id_prev();
        } else {
            player_.set_void_pose_in_game_over();
            id_next_ = SceneId::Title;
        }
        talkflow_listener_.clear_game_request();
    }

    player_.update_animation(false, false);

    if ( talkflow_.has_finished() ) {
        talkflow_.reset_state();
        set_scene_change_request(id_next_);
        player_.revive();
    }
}

void Scene_GameOver::draw(mgc::graphics::Framebuffer& fb) {
    fb.clear(MGC_COLOR_BLACK);
    talkflow_.draw(fb);
    player_.draw(fb);
}

} // namespace app

