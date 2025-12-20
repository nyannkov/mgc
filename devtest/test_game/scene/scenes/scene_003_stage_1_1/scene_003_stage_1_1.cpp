#include "scene_003_stage_1_1.hpp"
#include "scene/scene_helper.hpp"
#include "resources/mml/mml.h"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/font/k8x12.h"

namespace app {

Scene003_Stage1_1::Scene003_Stage1_1(GameContext& ctx) 
            : SceneBase(ctx),
              stage_(scx_.timer),
              scene_objects_(scx_) {

    id_ = SceneId::Id_003_Stage1_1;
    id_next_ = SceneId::Id_003_Stage1_1;

    scene::setup_scene_context(
        scx_,
        &stage_,
        &scene_objects_,
        nullptr //&event_objects_
    );
}

void Scene003_Stage1_1::init() {
    
    scx_.player.set_position({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(30)});

    stage_.setup(stage::StageId::Stage1_1);
    stage_.set_position({0, 0});

    camera_.set_target(scx_.player);
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(1));
    camera_.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(3));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(true);

    camera_.update_follow_position();

    scene_objects_.init();

    status_display_request_.request_show();

    scx_.sound.play_background_music(MML_BGM_3_STAGE1, 0.0);
}

void Scene003_Stage1_1::update() {

    scene::update(scx_, talkflow_, &camera_);

    scene_change_request_ = 
        scene::check_scene_transition_request(
            scx_,
            game_over_effect_,
            id_next_
        );
}

void Scene003_Stage1_1::draw(mgc::graphics::Framebuffer& fb) {

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

