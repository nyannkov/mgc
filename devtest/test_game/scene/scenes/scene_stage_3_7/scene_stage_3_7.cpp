#include "scene_stage_3_7.hpp"
#include "scene/scene_core.hpp"
#include "resources/mml/mml.h"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/font/k8x12.h"

namespace app {

Scene_Stage3_7::Scene_Stage3_7(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_) {

    set_id(SceneId::Stage3_7);

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        nullptr
    );
}

void Scene_Stage3_7::init() {
    
    if ( id_prev() == SceneId::Stage3_6 ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(60)},
            PlayerAnimState::StandRight
        );
    } else if ( id_prev() == SceneId::Stage4_1 ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(7)},
            PlayerAnimState::StandLeft
        );
    } else {
        //TODO
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(60)},
            PlayerAnimState::StandLeft
        );
    }

    scx_.sound.stop_background_music();

    stage_.setup(stage::StageId::Stage3_7);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_target_position_in_camera_space(
        {TARGET_X_IN_CAMERA, TARGET_Y_IN_CAMERA}
    );
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(24));
    camera_.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(59));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(true);

    camera_.snap_to_target();

    scene_objects_.init();

    status_display_request_.request_show();
}

void Scene_Stage3_7::update() {

    update_common(&camera_);
}

void Scene_Stage3_7::draw(mgc::graphics::Framebuffer& fb) {

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

