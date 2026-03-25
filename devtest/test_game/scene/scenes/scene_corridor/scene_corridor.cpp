#include "scene_corridor.hpp"
#include "scene/scene_core.hpp"
#include "resources/mml/mml.h"

namespace app {

Scene_Corridor::Scene_Corridor(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_),
              event_objects_(scx_, scene_objects_) {

    set_id(SceneId::Corridor);

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        &event_objects_
    );
}

void Scene_Corridor::init() {
    
    switch ( id_prev() ) {
    case SceneId::TowerFront:
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(10)},
            PlayerAnimState::StandRight
        );
        break;
    case SceneId::Stage1_1:
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(36), MGC_CELL2PIXEL(10)},
            PlayerAnimState::StandLeft
        );
        break;
    default:
        break;
    }
    scx_.sound.stop_background_music();

    stage_.setup(stage::StageId::Corridor);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(1));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(false);

    camera_.update_follow_position();

    scene_objects_.init();
    event_objects_.init();

    status_display_request_.request_show();
}

void Scene_Corridor::update() {

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

void Scene_Corridor::draw(mgc::graphics::Framebuffer& fb) {

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

