#include "scene_stage_1_3.hpp"
#include "scene/scene_core.hpp"
#include "resources/mml/mml.h"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/font/k8x12.h"

namespace app {

Scene_Stage1_3::Scene_Stage1_3(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_) {

    set_id(SceneId::Stage1_3);

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        nullptr //&event_objects_
    );
}

void Scene_Stage1_3::init() {
    
    if ( id_prev() == SceneId::Stage1_4 ) {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(21), MGC_CELL2PIXEL(22)},
            PlayerAnimState::StandRight
        );
    } else {
        scx_.player.reset_state_for_placement(
            {MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(18)},
            PlayerAnimState::StandRight
        );
    }

    set_background_music(MML_BGM_3_STAGE1);

    stage_.setup(stage::StageId::Stage1_3);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(1));
    camera_.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(3));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(true);

    camera_.update_follow_position();

    scene_objects_.init();

    status_display_request_.request_show();
}

void Scene_Stage1_3::update() {

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

void Scene_Stage1_3::draw(mgc::graphics::Framebuffer& fb) {

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

