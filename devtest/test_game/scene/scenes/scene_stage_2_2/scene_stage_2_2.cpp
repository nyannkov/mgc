#include "scene_stage_2_2.hpp"
#include "scene/scene_core.hpp"
#include "resources/mml/mml.h"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/font/k8x12.h"

namespace app {

Scene_Stage2_2::Scene_Stage2_2(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_) {

    set_id(SceneId::Stage2_2);

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        nullptr
    );
}

void Scene_Stage2_2::init() {
    
    if ( id_prev() == SceneId::Stage2_1 ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(72)},
            PlayerAnimState::StandRight
        );
    } else if ( id_prev() == SceneId::Stage2_3 ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(2)},
            PlayerAnimState::StandRight
        );
    } else if ( id_prev() == SceneId::Stage2_4 ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(28), MGC_CELL2PIXEL(2)},
            PlayerAnimState::StandRight
        );
    } else {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(72)},
            PlayerAnimState::StandLeft
        );
    }

    set_background_music(MML_BGM_6_STAGE2);

    stage_.setup(stage::StageId::Stage2_2);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_target_position_in_camera_space(
        {TARGET_X_IN_CAMERA, TARGET_Y_IN_CAMERA}
    );
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(23));
    camera_.set_y_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(76));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(true);

    camera_.snap_to_target();

    scene_objects_.init();

    status_display_request_.request_show();
}

void Scene_Stage2_2::update() {

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

void Scene_Stage2_2::draw(mgc::graphics::Framebuffer& fb) {

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

