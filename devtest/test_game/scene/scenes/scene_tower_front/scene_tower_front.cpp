#include "scene_tower_front.hpp"
#include "scene/scene_core.hpp"
#include "resources/mml/mml.h"

namespace app {

Scene_TowerFront::Scene_TowerFront(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_),
              event_objects_(scx_, scene_objects_) { 

    set_id(SceneId::TowerFront);

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        &event_objects_
    );
}

void Scene_TowerFront::init() {

    if ( id_prev() == SceneId::Corridor ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(56), MGC_CELL2PIXEL(10)},
            PlayerAnimState::StandLeft
        );
    } else {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(10)},
            PlayerAnimState::StandRight
        );
    }

    set_background_music(MML_BGM_2_WATER_FALL);

    stage_.setup(stage::StageId::TowerFront);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(52), MGC_CELL2PIXEL(1));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(false);

    camera_.update_follow_position();

    scene_objects_.init();
    event_objects_.init();

    status_display_request_.request_show();
}

void Scene_TowerFront::update() {

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

void Scene_TowerFront::draw(mgc::graphics::Framebuffer& fb) {

    scene::draw(
        MGC_COLOR(0.1, 0.1, 0.5), 
        fb,
        &camera_,
        scx_,
        talkflow_,
        game_over_effect_
    );
}

} // namespace app

