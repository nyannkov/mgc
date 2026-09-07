#include "scene_log_terminal.hpp"
#include "scene/scene_core.hpp"
#include "resources/mml/mml.h"

namespace app {

Scene_LogTerminal::Scene_LogTerminal(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_),
              event_objects_(scx_, scene_objects_) {

    set_id(SceneId::LogTerminal);

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        &event_objects_
    );
}

void Scene_LogTerminal::init() {
    
    scx_.player.reset_state_for_placement(
        {MGC_CELL2PIXEL(22), MGC_CELL2PIXEL(10)},
        PlayerAnimState::StandRight
    );

    scx_.sound.stop_background_music();

    stage_.setup(stage::StageId::LogTerminal);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_target_position_in_camera_space(
        {TARGET_X_IN_CAMERA+16, TARGET_Y_IN_CAMERA}
    );
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(22));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(false);

    camera_.snap_to_target();

    scene_objects_.init();
    event_objects_.init();

    status_display_request_.request_show();
}

void Scene_LogTerminal::update() {

    SceneId id_next;
    
    if ( scx_.gamepad.just_pressed(mgc::platform::input::Key::Menu) ) {
        set_menu_request();
        return;
    }

    scene::update(scx_, talkflow_, &camera_);

    bool r = scene::check_scene_transition_request(
            scx_,
            game_over_effect_,
            id_next
    );

    if ( r ) {
        set_scene_change_request(id_next);
    }
}

void Scene_LogTerminal::draw(mgc::graphics::Framebuffer& fb) {

    scene::draw(
        MGC_COLOR_BLACK,
        fb,
        &camera_,
        scx_,
        talkflow_,
        game_over_effect_
    );
}

} // namespace app

