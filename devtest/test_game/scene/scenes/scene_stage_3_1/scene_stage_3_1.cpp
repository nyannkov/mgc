#include "scene_stage_3_1.hpp"
#include "scene/scene_core.hpp"
#include "resources/mml/mml.h"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/font/k8x12.h"

namespace app {

Scene_Stage3_1::Scene_Stage3_1(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_) {

    set_id(SceneId::Stage3_1);

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        nullptr
    );
}

void Scene_Stage3_1::init() {
    
    if ( id_prev() == SceneId::Stage2_9 ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(27)},
            PlayerAnimState::StandRight
        );
    } else {
        //TODO
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(27)},
            PlayerAnimState::StandLeft
        );
    }

    set_background_music(MML_BGM_7_STAGE3);

    stage_.setup(stage::StageId::Stage3_1);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(1));
    camera_.set_y_follow_setting(MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(1));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(true);

    camera_.update_follow_position();

    scene_objects_.init();

    status_display_request_.request_show();
}

void Scene_Stage3_1::update() {

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

void Scene_Stage3_1::draw(mgc::graphics::Framebuffer& fb) {

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

