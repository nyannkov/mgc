#include "scene_000.hpp"
#include "resources/generated/font/k8x12.h"
#include "resources/mml/mml.h"

namespace app {

using mgc::platform::input::Key;

Scene000::Scene000(GameContext& ctx)
                : SceneBase(ctx) {
    id_ = SceneId::Id_000;
    id_next_ = SceneId::Id_000;
}

void Scene000::init() {
    label_title_.set_position({16*4, 16});
    label_title_.set_font(k8x12);
    label_title_.set_fontsize2x(true);
    label_title_.set_font_fore_color(MGC_COLOR_WHITE);
    label_title_.set_text("テストゲーム");

    label_title_en_.set_position({16*4+8, 16*3});
    label_title_en_.set_font(k8x12);
    label_title_en_.set_fontsize2x(true);
    label_title_en_.set_font_fore_color(MGC_COLOR_WHITE);
    label_title_en_.set_text("TEST GAME");

    selectbox_menu_.set_position({16*5, 16*7});
    selectbox_menu_.set_font(k8x12);
    selectbox_menu_.set_padding({8, 8, 8, 8});
    selectbox_menu_.set_size({60, 30});
    selectbox_menu_.add_item("START");
    selectbox_menu_.add_item("PASSWORD");

    status_display_request_.request_hide();

    scx_.sound.stop_background_music(0.0);

    scx_.checkpoint_info.set_checkpoint(Checkpoint::Initial);

    scx_.player.init();
}

void Scene000::update() {
    if ( scx_.gamepad.just_pressed(Key::Up) ) {
        selectbox_menu_.select_previous();

    } else if ( scx_.gamepad.just_pressed(Key::Down) ) {
        selectbox_menu_.select_next();

    } else if ( scx_.gamepad.just_pressed(Key::Enter) ) {
        if ( selectbox_menu_.selected_index() == 0 ) {
            scx_.sound.play_sound_effect(MML_SE_0_MEOW, 0.0);
            change_wait_ = true;
        }

    } else { }

    if ( change_wait_ ) {
        if ( scx_.sound.have_all_sound_effects_finished() ) {
            id_next_ = SceneId::Id_001_TowerFront;
            scene_change_request_ = true;
        }
    }
}

void Scene000::draw(mgc::graphics::Framebuffer& fb) {
    fb.clear(MGC_COLOR_BLACK);
    label_title_.draw(fb);    
    label_title_en_.draw(fb);    
    selectbox_menu_.draw(fb);    
}

} // namespace app

