#include "scene_001.hpp"
#include "resources/mml/mml.h"
#include "resources/generated/talkscript/test_talkscript.h"
#include "resources/generated/font/k8x12.h"

namespace app {

Scene001::Scene001(GameContext& ctx) 
            : id_(SceneId::Id_001),
              id_next_(SceneId::Id_001),
              frame_timer_(ctx.frame_timer()),
              sound_controller_(ctx.sound_controller()),
              player_(ctx.player()),
              status_display_request_(ctx.status_display_request()),
              stage_info_(ctx.stage_info()),
              change_request_(false),
              enemies_(ctx),
              items_(ctx),
              props_(ctx),
              main_layer_(ctx.frame_timer()),
              back1_layer_(ctx.frame_timer()),
              back2_layer_(ctx.frame_timer()),
              talkflow_controller_(ctx.gamepad()),
              talkflow_listener_(ctx.player()) {
}

void Scene001::init() {

    stage_info_.set_current_scene_id(SceneId::Id_001);

    if ( player_.is_game_over() ) {
        player_.clear_game_over();
        player_.revive();
        sound_controller_.play_background_music(MML_BGM_0_THE_FLEA_WALTZ, 0.0);
    } else {
        if ( stage_info_.previous_scene_id() == SceneId::Id_000 ) {
            sound_controller_.play_background_music(MML_BGM_0_THE_FLEA_WALTZ, 0.0);
        }
    }

    sound_controller_.shift_pitch_by_degree(15*stage_info_.current_floor());

    main_layer_.set_layer(CollisionTileLayerId::Layer_001);
    main_layer_.set_position({0, 0});

    back1_layer_.set_layer(TileLayerId::Layer_001_Background_1);
    back1_layer_.set_position({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(2)});
    back1_layer_.set_parallax_factor({1, 1});

    back2_layer_.set_layer(TileLayerId::Layer_001_Background_2);
    back2_layer_.set_position({MGC_CELL2PIXEL(0), static_cast<int16_t>(MGC_CELL2PIXEL(11) + stage_info_.current_floor())});
    back2_layer_.set_parallax_factor({0.9, 0.1});

    
    if ( stage_info_.current_floor() >= stage_info_.previous_floor() ) {
        player_.set_position({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(38)});
    } else {
        player_.set_position({MGC_CELL2PIXEL(18), MGC_CELL2PIXEL(5)});
    }

    enemies_.init();

    items_.init();

    props_.init();

    camera_.set_target(player_);
    camera_.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(14), MGC_CELL2PIXEL(3));
    camera_.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(3));
    camera_.set_x_follow_enabled(true);
    camera_.set_y_follow_enabled(true);

    camera_.update_follow_position();

    talkflow_controller_.bind_listener(talkflow_listener_);
    talkflow_controller_.set_font(k8x12);
    talkflow_controller_.set_talkscript(test_talkscript);
    talkflow_controller_.set_selectbox_config(
        {
            {170, 30}, // position
            {48, 30}, // size
            "*", // cursor simbol
            {4, 0}, // cursor offset
        }
    );
    talkflow_controller_.set_dialoguebox_config(
        {
            {8, 8}, // position
            {216, 36}, // size
            1, // scroll speed
            1, // scroll threshold line
            3, // line spacing
            4, // typing speed
        }
    );

    // Show status display.
    status_display_request_.request_show();
}

void Scene001::update() {

    player_.pre_update();

    for ( auto& e : enemies_.all() ) e->pre_update();

    col_detector_.detect(player_, main_layer_);

    for ( auto& e : enemies_.all() ) {
        col_detector_.detect(*e, main_layer_);
        mgc::collision::CollisionDetectorBoxToBox::detect(player_, *e);
        mgc::collision::CollisionDetectorBoxToBox::detect(player_.attack(), *e);
    }

    for ( auto& item : items_.all() ) {
        mgc::collision::CollisionDetectorBoxToBox::detect(player_, *item);
    }

    for ( auto& prop : props_.all() ) {
        mgc::collision::CollisionDetectorBoxToBox::detect(player_, *prop);
    }

    player_.post_update();

    for ( auto& e : enemies_.all() ) e->post_update();

    camera_.update_follow_position();

    if ( stage_info_.next_scene_request() ) {
        stage_info_.clear_next_scene_request();
        id_next_ = stage_info_.next_scene_id();
        change_request_ = true;
    }
}

void Scene001::draw(mgc::graphics::Framebuffer& fb) {

    fb.clear(MGC_COLOR(0.1, 0.1, 0.5));
    
    const auto pos = camera_.follow_position();

    back2_layer_.draw(fb, pos);
    back1_layer_.draw(fb, pos);
    main_layer_.draw(fb, pos);

    for ( auto& prop: props_.all() ) prop->draw(fb, pos);

    for ( auto& item: items_.all() ) item->draw(fb, pos);

    for ( auto& e : enemies_.all() ) e->draw(fb, pos);

    // TODO separate
    if ( player_.is_game_over() ) {
        if ( !sound_controller_.is_background_music_playing() ) {
            if ( screen_fader_.fade_state() == ScreenFaderState::None ) {
                screen_fader_.request_fade_out(fb);
                talkflow_controller_.begin(MGC_TEST_TALKSCRIPT_GAME_OVER);
            }
            screen_fader_.update(fb);
            if ( screen_fader_.fade_state() == ScreenFaderState::FadeOutComplete ) {
                talkflow_controller_.proc();
                if ( talkflow_controller_.has_finished() ) {
                    if ( talkflow_listener_.continue_requested() ) {
                        id_next_ = SceneId::Id_001;
                    } else {
                        id_next_ = SceneId::Id_000; // Go to title
                    }
                    stage_info_.set_current_floor(0);
                    change_request_ = true;
                }
                talkflow_controller_.draw(fb, pos, nullptr);
            }
        }
    }
    player_.draw(fb, pos);

    player_.attack().draw(fb, pos);
}


void Scene001::TalkflowListener::on_choice_done(
    mgc_node_idx_t tag,
    size_t item_tag,
    int32_t value
) {
    if ( tag == MGC_TEST_TALKSCRIPT_TAG_CHOICE__TRY_AGAIN ) {
        if ( item_tag == MGC_TEST_TALKSCRIPT_TAG_CHOICE__TRY_AGAIN__ITEM__YES ) {
            player_.set_power_pose_in_game_over();
            continue_requested_ = true;
        } else {
            player_.set_void_pose_in_game_over();
            continue_requested_ = false;
        }
    }
}

} // namespace app

