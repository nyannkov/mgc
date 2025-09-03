#ifndef MGC_SCENE_000_HPP
#define MGC_SCENE_000_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_base.hpp"
#include "game_context/game_context.hpp"
#include "resources/generated/font/k8x12.h"

namespace app {

using mgc::platform::input::Key;

struct Scene000 : SceneBase {

    explicit Scene000(GameContext& ctx) 
                : gamepad_(ctx.gamepad()),
                  id_(SceneId::Id_000),
                  id_next_(SceneId::Id_000),
                  change_request_(false)
                  { }

    SceneId id() const override { return id_; }
    SceneId id_next() const override { return id_next_; }
    bool has_scene_change_request() const override { return change_request_; };

    void init() override {
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
        selectbox_menu_.add_item("START");
        selectbox_menu_.add_item("SCORE");
    }

    void update() override {
        
        if ( gamepad_.just_pressed(Key::Up) ) {
            selectbox_menu_.select_previous();

        } else if ( gamepad_.just_pressed(Key::Down) ) {
            selectbox_menu_.select_next();

        } else if ( gamepad_.just_pressed(Key::Enter) ) {
            
            if ( selectbox_menu_.selected_index() == 0 ) {
                id_next_ = SceneId::Id_001;
                change_request_ = true;
            }

        } else {
        }
    }

    void draw(mgc::graphics::Framebuffer& fb) override {
        fb.clear(MGC_COLOR_BLACK);
        label_title_.draw(fb);    
        label_title_en_.draw(fb);    
        selectbox_menu_.draw(fb);    
    }

private:
    const GamepadT& gamepad_;
    const SceneId id_; 
    SceneId id_next_;
    bool change_request_;
    mgc::parts::BasicLabel label_title_;
    mgc::parts::BasicLabel label_title_en_;
    mgc::parts::BasicSelectbox selectbox_menu_;
};

} // namespace app

#endif/*MGC_SCENE_000_HPP*/

