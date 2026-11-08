#include "scene_stage_4_2.hpp"
#include "scene/scene_core.hpp"
#include "resources/mml/mml.h"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/font/k8x12.h"

namespace app {

Scene_Stage4_2::Scene_Stage4_2(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_),
              event_objects_(scx_, scene_objects_, camera_) {

    set_id(SceneId::Stage4_2);

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        &event_objects_
    );
}

void Scene_Stage4_2::init() {
    
    if ( id_prev() == SceneId::Shop ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(47), MGC_CELL2PIXEL(31)},
            PlayerAnimState::StandRight
        );
    } else if ( id_prev() == SceneId::Stage4_1 ) {
        
        switch ( from_gate_id() ) {
        case 2:
            scx_.player.reset_state_for_placement(
                {MGC_CELL2PIXEL(22), MGC_CELL2PIXEL(18)},
                PlayerAnimState::StandRight
            );
            break;
        case 3:
            scx_.player.reset_state_for_placement(
                {MGC_CELL2PIXEL(12), MGC_CELL2PIXEL(71)},
                PlayerAnimState::StandRight
            );
            break;
        case 4:
            scx_.player.reset_state_for_placement(
                {MGC_CELL2PIXEL(121), MGC_CELL2PIXEL(47)},
                PlayerAnimState::StandRight
            );
            break;
        default:
            scx_.player.reset_state_for_placement(
                {MGC_CELL2PIXEL(22), MGC_CELL2PIXEL(18)},
                PlayerAnimState::StandRight
            );
            break;
        }

    } else {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(28), MGC_CELL2PIXEL(30)},
            PlayerAnimState::StandLeft
        );
    }

    set_background_music(MML_BGM_10_STAGE4);

    stage_.setup(stage::StageId::Stage4_2);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_target_position_in_camera_space(
        {TARGET_X_IN_CAMERA, TARGET_Y_IN_CAMERA}
    );
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(155));
    camera_.set_y_follow_setting(MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(75));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(true);

    camera_.snap_to_target();

    scene_objects_.init();
    event_objects_.init();

    status_display_request_.request_show();
}

void Scene_Stage4_2::update() {

    update_common(&camera_);
}

void Scene_Stage4_2::draw(mgc::graphics::Framebuffer& fb) {

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

