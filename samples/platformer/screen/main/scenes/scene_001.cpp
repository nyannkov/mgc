/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include <cstdio>
#include <memory>
#include "scene_001.hpp"
#include "system/system.h"
#include "actor/player.hpp"
#include "screen/sub/dlgflow_handler_sub_screen.h"
#include "resources/generates/tileset/tileset_static_objects.h"
#include "resources/generates/tileset/tileset_map_elements.h"
#include "resources/generates/map/map_001.h"
#include "resources/generates/map/map_001_bg.h"

namespace mgc::main_screen {

static const char * item_array_yesno[2] = { "はい", "いいえ" };
static const mgc_dlg_items_t items_yesno = { item_array_yesno, countof(item_array_yesno) };
static const mgc_dlgnode_t node_array[] = {
    { .id=0x00, .type=MGC_DLG_TYPE_TEXT, .params={.text="\n            〜このサンプルについて〜"} },
    { .id=0x01, .type=MGC_DLG_TYPE_TEXT, .params={.text="このサンプルは開発進捗に応じて内容が変更される場合がございます。"}, .terminal=true },
    { .id=0x10, .type=MGC_DLG_TYPE_TEXT, .params={.text="外に出ますか？" }, .flags=0x01 },
    { .id=0x11, .type=MGC_DLG_TYPE_SELECT, .params={.items=&items_yesno },
        .cb_before_switch_node = [](const mgc_dlgflow_t *dlgflow) -> mgc_node_id_t { return ( dlgflow->result == 0 ) ? 0x20 : 0x21; }
    },
    { .id=0x20, .type=MGC_DLG_TYPE_TEXT, .params={.text=""}, .terminal=true, .flags=0x01 },
    { .id=0x21, .type=MGC_DLG_TYPE_TEXT, .params={.text=""}, .terminal=true, .flags=0x01 },
};

void Scene001::init(SceneId prev_id) {
    auto sound = sys_get_sound_driver();
    next_id_ = SceneId::ID_001;
    scene_trans_ = false;
    scene_state_ = SceneState::Showing;
    cell_mask_ = (int16_t)(MGC_CELL_LEN - 1)*-1;
    maphit_init(&maphit_);
    sprhit_init(&sprhit_);
    dlgflow_init(&dlgflow_, &dlgflow_handler_sub_screen);
    this->init_components(prev_id);

    camera_update(&pixelbuffer_, &camera_, player_.get_ptr_sprite());
}

SceneId Scene001::get_id() const {
    return SceneId::ID_001;
}

SceneId Scene001::get_next_id() const {
    return next_id_;
}

bool Scene001::check_trans() const {
    return scene_trans_;
}

void Scene001::update() {
    auto gamepad = sys_get_gamepad_driver();
    bool is_key1_on_edge = gamepad->is_key_on_edge(GP_KEY_1);
    mgc_dlgflow_state flow_state = dlgflow_get_state(&dlgflow_);

    switch ( flow_state ) {
    case MGC_DLG_FLOW_STOP:
        if ( scene_state_ == SceneState::Shown ) {
            player_.update();
            check_hit_tilemap(&player_, maphit_, tilemap_);
            if ( check_hit_sprite(&player_, sprhit_, *signboard_.get_ptr_sprite()) ) {
                if ( is_key1_on_edge == true ) {
                    dlgflow_set_node_array(&dlgflow_, node_array, countof(node_array), 0x00);
                }
            }
            if ( check_hit_sprite(&player_, sprhit_, *gate_.get_ptr_sprite()) ) {
                if ( is_key1_on_edge == true ) {
                    dlgflow_set_node_array(&dlgflow_, node_array, countof(node_array), 0x10);
                }
            }
            camera_update(&pixelbuffer_, &camera_, player_.get_ptr_sprite());
            player_.update_anim();
        }
        break;

    case MGC_DLG_FLOW_READY:/*FALLTHROUGH*/
    case MGC_DLG_FLOW_NODE_PROGRESSING:
        dlgflow_run_node_proc(&dlgflow_);
        break;

    case MGC_DLG_FLOW_NODE_COMPLETED:
        if ( dlgflow_is_flow_terminated(&dlgflow_) ) {
            mgc_node_id_t node_id = dlgflow_get_current_node_id(&dlgflow_);
            if ( node_id == 0x20 ) {
                next_id_ = SceneId::ID_002;
                scene_state_ = SceneState::Closing;
                cell_mask_ = 0;
            }
            dlgflow_clear_state(&dlgflow_);
        } else {
            dlgflow_switch_to_next_node(&dlgflow_);
        }
        break;

    case MGC_DLG_FLOW_ERROR:/*FALLTHROUGH*/
    default:
        dlgflow_clear_state(&dlgflow_);
        break;
    }
}

void Scene001::draw(uint16_t screen_x0, uint16_t screen_y0, uint16_t width, uint16_t height) {
    for ( int16_t y = 0; y < height; y += MGC_CELL2PIXEL(1)) {
        for ( int16_t x = 0; x < width; x += MGC_CELL2PIXEL(1)) {
            tilemap_apply_cell_blending(&tilemap_bg_, &pixelbuffer_, x, y);
            tilemap_apply_cell_blending(&tilemap_, &pixelbuffer_, x, y);
            sprite_apply_cell_blending(gate_.get_ptr_sprite(), &pixelbuffer_, x, y);
            sprite_apply_cell_blending(signboard_.get_ptr_sprite(), &pixelbuffer_, x, y);
            sprite_apply_cell_blending(player_.get_ptr_sprite(), &pixelbuffer_, x, y);
            if ( scene_state_ != SceneState::Shown ) {
                pixelbuffer_fill_partial_with_color(&pixelbuffer_, 0, cell_mask_, cell_mask_);
            }
            pixelbuffer_draw_cell(&pixelbuffer_, sys_get_display_driver(), screen_x0+x, screen_y0+y);
        }
    }
    switch (scene_state_) {
    case SceneState::Closing:
        cell_mask_++;
        if ( cell_mask_ >= MGC_CELL_LEN ) {
            scene_state_ = SceneState::Closed;
            scene_trans_ = true;
        }
        break;
    case SceneState::Showing:
        cell_mask_++;
        if ( cell_mask_ >= 0 ) {
            cell_mask_ = 0;
            scene_state_ = SceneState::Shown;
        }
        break;
    default:
        break;
    }
}

void Scene001::init_components(SceneId prev_id) {
    // pixelbuffer
    pixelbuffer_init(&pixelbuffer_);

    // player
    if ( prev_id == SceneId::ID_002 ) {
        player_.init(0, MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(4));
        player_.set_anim(AnimType::Stop, AnimDir::Left);
    } else {
        player_.init(0, MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(9));
        player_.set_anim(AnimType::Stop, AnimDir::Right);
    }

    // camera
    camera_init(&camera_);
    camera_set_x_follow_settings(&camera_, MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(map_001.map_width-10), MGC_CELL2PIXEL(3));
    camera_set_y_follow_settings(&camera_, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(1));
    camera_set_x_follow_enabled(&camera_, true);
    camera_set_y_follow_enabled(&camera_, false);

    // gate
    gate_.init(0, MGC_CELL2PIXEL(20)-8, MGC_CELL2PIXEL(4));
    gate_.init_hitbox(0, 2, 2, 12, 12);
    gate_.set_tileset(tileset_static_objects);
    gate_.set_tile_idx(2);

    // signboard
    signboard_.init(0, MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(5));
    signboard_.init_hitbox(0, 2, 2, 12, 12);
    signboard_.set_tileset(tileset_static_objects);
    signboard_.set_tile_idx(1);

    // tilemap
    tilemap_init(&tilemap_, 1, &map_001, &tileset_map_elements);
    tilemap_set_position(&tilemap_, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(1));
    tilemap_set_r_cell_offset(&tilemap_, 1, 1);
    tilemap_set_enabled(&tilemap_, true);

    // tilemap (background)
    tilemap_init(&tilemap_bg_, 1, &map_001_bg, &tileset_map_elements);
    tilemap_set_position(&tilemap_bg_, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(1));
    tilemap_set_r_cell_offset(&tilemap_bg_, 2, 1);
    tilemap_set_enabled(&tilemap_bg_, true);
}

}
