#include "scene_000.hpp"
#include "resources/generated/font/k8x12.h"
#include "resources/mml/mml.h"

namespace app {

using mgc::platform::input::Key;

Scene000::Scene000(GameContext& ctx)
                : gamepad_(ctx.gamepad()),
                  sound_controller_(ctx.sound_controller()),
                  status_display_request_(ctx.status_display_request()),
                  id_(SceneId::Id_000),
                  id_next_(SceneId::Id_000),
                  change_request_(false),
                  change_wait_(false) {
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
    selectbox_menu_.add_item("SCORE");

    status_display_request_.request_hide();

    sound_controller_.stop_background_music(0.0);
}

void Scene000::update() {
    if ( gamepad_.just_pressed(Key::Up) ) {
        selectbox_menu_.select_previous();

    } else if ( gamepad_.just_pressed(Key::Down) ) {
        selectbox_menu_.select_next();

    } else if ( gamepad_.just_pressed(Key::Enter) ) {
        if ( selectbox_menu_.selected_index() == 0 ) {
            sound_controller_.play_sound_effect(MML_SE_0_MEOW, 0.0);
            change_wait_ = true;
        }

    } else { }

    if ( change_wait_ ) {
        if ( sound_controller_.have_all_sound_effects_finished() ) {
            id_next_ = SceneId::Id_001;
            change_request_ = true;
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

