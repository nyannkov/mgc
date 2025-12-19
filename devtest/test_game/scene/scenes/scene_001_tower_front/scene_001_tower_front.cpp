#include "scene_001_tower_front.hpp"
#include "scene/scene_helper.hpp"
#include "resources/mml/mml.h"

namespace app {

Scene001_TowerFront::Scene001_TowerFront(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_),
              event_objects_(scx_, scene_objects_) { 

    id_ = SceneId::Id_001_TowerFront;
    id_next_ = SceneId::Id_001_TowerFront;

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        &event_objects_
    );
}

void Scene001_TowerFront::init() {

    if ( id_prev_ == SceneId::Id_002_Corridor ) {
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

    stage_.setup(stage::StageId::TowerFront);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(51), MGC_CELL2PIXEL(1));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(false);

    camera_.update_follow_position();

    scene_objects_.init();
    event_objects_.init();

    status_display_request_.request_show();

    scx_.sound.play_background_music(MML_BGM_2_WATER_FALL, 0.0);
}

void Scene001_TowerFront::update() {
    
    scene::update(scx_, talkflow_, &camera_);

    scene_change_request_ = 
        scene::check_scene_transition_request(
            scx_,
            game_over_effect_,
            id_next_
        );
}

void Scene001_TowerFront::draw(mgc::graphics::Framebuffer& fb) {

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

