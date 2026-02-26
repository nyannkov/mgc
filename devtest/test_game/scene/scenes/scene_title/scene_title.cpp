#include "scene_title.hpp"
#include "resources/generated/font/k8x12.h"
#include "resources/mml/mml.h"

namespace app {

using mgc::platform::input::Key;

Scene_Title::Scene_Title(GameContext& ctx)
                : SceneBase(ctx) {

    set_id(SceneId::Title);
}

void Scene_Title::init() {
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

void Scene_Title::update() {

    switch (selected_state_) {
    case SelectedState::Start:
        if ( scx_.sound.have_all_sound_effects_finished() ) {
            set_scene_change_request(SceneId::TowerFront);
        }
        break;
    case SelectedState::Password:
        set_scene_change_request(SceneId::Password);
        break;
    default:
        update_select();
        break;
    }
}

void Scene_Title::draw(mgc::graphics::Framebuffer& fb) {
    fb.clear(MGC_COLOR_BLACK);
    label_title_.draw(fb);    
    label_title_en_.draw(fb);    
    selectbox_menu_.draw(fb);    
}

void Scene_Title::update_select() {
    if ( scx_.gamepad.just_pressed(Key::Up) ) {
        selectbox_menu_.select_previous();
    } else if ( scx_.gamepad.just_pressed(Key::Down) ) {
        selectbox_menu_.select_next();
    } else if ( scx_.gamepad.just_pressed(Key::Enter) ) {
        switch (selectbox_menu_.selected_index()) {
        case SELECT_INDEX_START:
            scx_.sound.play_sound_effect(MML_SE_0_MEOW, 0.0);
            selected_state_ = SelectedState::Start;
            break;
        case SELECT_INDEX_PASSWORD:
            selected_state_ = SelectedState::Password;
            break;
        default:
            break;
        }
    } else { }
}

} // namespace app

