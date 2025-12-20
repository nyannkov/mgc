#include "scene_002_corridor.hpp"
#include "scene/scene_helper.hpp"
#include "resources/mml/mml.h"

namespace app {

Scene002_Corridor::Scene002_Corridor(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_),
              event_objects_(scx_, scene_objects_) {

    id_ = SceneId::Id_002_Corridor;
    id_next_ = SceneId::Id_002_Corridor;

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        &event_objects_
    );
}

void Scene002_Corridor::init() {
    
    if ( id_prev_ == SceneId::Id_003_Stage1_1 ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(36), MGC_CELL2PIXEL(10)},
            PlayerAnimState::StandLeft
        );
    } else {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(10)},
            PlayerAnimState::StandRight
        );
    }


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

    scx_.sound.stop_background_music(0.0);
}

void Scene002_Corridor::update() {
    
    scene::update(scx_, talkflow_, &camera_);

    scene_change_request_ = 
        scene::check_scene_transition_request(
            scx_,
            game_over_effect_,
            id_next_
        );
}

void Scene002_Corridor::draw(mgc::graphics::Framebuffer& fb) {

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

