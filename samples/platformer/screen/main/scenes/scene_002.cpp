/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include <cstdio>
#include <memory>
#include "scene_002.hpp"
#include "system/system.h"
#include "actor/player.hpp"
#include "screen/sub/sub_screen_handler.h"
#include "resources/generates/tileset/tileset_static_objects.h"
#include "resources/generates/tileset/tileset_map_elements.h"
#include "resources/generates/map/map_002.h"
#include "resources/generates/map/map_002_bg.h"

namespace mgc::main_screen {

const mgc_dlg_node node_array[] = {
    {0, MGC_DLG_TRUE, "工事中", MGC_DLG_ON_BUTTON_PRESSED, MGC_DLG_EXIT, 0},
    {10, MGC_DLG_TRUE, "戻りますか？", MGC_DLG_ON_RECEIVE_YESNO, MGC_DLG_STEP, 0},
    {11, MGC_DLG_IF_YES, "", MGC_DLG_ON_CURSOR_END, MGC_DLG_EXIT, 0},
    {12, MGC_DLG_IF_NO, "", MGC_DLG_ON_CURSOR_END, MGC_DLG_EXIT, 0},
};

const mgc_dlg_flow flow_0 = {
    .flow_id = 0,
    .node_array = node_array,
    .node_count = sizeof(node_array)/sizeof(node_array[0]),
};

void Scene002::init(SceneId prev_id) {
    auto sound = sys_get_sound_driver();
    next_id_ = SceneId::ID_001;
    scene_trans_ = false;
    scene_state_ = SceneState::Showing;
    cell_mask_ = (int16_t)(MGC_CELL_LEN - 1)*-1;
    maphit_init(&maphit_);
    sprhit_init(&sprhit_);
    dlgctrl_init(&dlgctrl_, &dlghdr_sub_screen);
    this->init_components(prev_id);

    camera_update(&pixelbuffer_, &camera_, player_.get_ptr_sprite());
}

SceneId Scene002::get_id() const {
    return SceneId::ID_002;
}

SceneId Scene002::get_next_id() const {
    return next_id_;
}

bool Scene002::check_trans() const {
    return scene_trans_;
}

void Scene002::update() {
    auto gamepad = sys_get_gamepad_driver();
    bool is_key1_on_edge = gamepad->is_key_on_edge(GP_KEY_1);
    mgc_dlg_flow_state flow_state = dlgctrl_get_flow_state(&dlgctrl_);

    if ( flow_state == MGC_DLG_FLOW_NOT_SET ) {
        if ( scene_state_ == SceneState::Shown ) {
            player_.update();
            check_hit_tilemap(&player_, maphit_, tilemap_);
            if ( check_hit_sprite(&player_, sprhit_, *signboard_.get_ptr_sprite()) ) {
                if ( is_key1_on_edge == true ) {
                    dlgctrl_set_flow(&dlgctrl_, &flow_0, 0);
                }
            }
            if ( check_hit_sprite(&player_, sprhit_, *gate_.get_ptr_sprite()) ) {
                if ( is_key1_on_edge == true ) {
                    dlgctrl_set_flow(&dlgctrl_, &flow_0, 10);
                }
            }
            camera_update(&pixelbuffer_, &camera_, player_.get_ptr_sprite());
            player_.update_anim();
        }
    } else if ( flow_state == MGC_DLG_FLOW_SUSPEND ) {
        dlgctrl_resume_flow(&dlgctrl_);
    } else if ( flow_state == MGC_DLG_FLOW_END ) {
        if ( dlgctrl_get_node_id(&dlgctrl_) == 11 ) {
            next_id_ = SceneId::ID_001;
            scene_state_ = SceneState::Closing;
            cell_mask_ = 0;
        }
        dlgctrl_clear_flow(&dlgctrl_);
    } else {
        dlgctrl_update_flow(&dlgctrl_);
    }
}

void Scene002::draw(uint16_t screen_x0, uint16_t screen_y0, uint16_t width, uint16_t height) {
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

void Scene002::init_components(SceneId prev_id) {
    (void)prev_id;
    // pixelbuffer
    pixelbuffer_init(&pixelbuffer_);
    pixelbuffer_set_back_color(&pixelbuffer_, MGC_COLOR(0.0, 1.0, 1.0));

    // player
    player_.init(0, MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(38));
    player_.set_anim(AnimType::Stop, AnimDir::Right);

    // camera
    camera_init(&camera_);
    camera_set_x_follow_settings(&camera_, MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(map_002.map_width-13), MGC_CELL2PIXEL(3));
    camera_set_y_follow_settings(&camera_, MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(map_002.map_height-10), MGC_CELL2PIXEL(3));
    camera_set_x_follow_enabled(&camera_, true);
    camera_set_y_follow_enabled(&camera_, true);

    // gate
    gate_.init(0, MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(38));
    gate_.init_hitbox(0, 2, 2, 12, 12);
    gate_.set_tileset(tileset_static_objects);
    gate_.set_tile_idx(3);

    // signboard
    signboard_.init(0, MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(2));
    signboard_.init_hitbox(0, 2, 2, 12, 12);
    signboard_.set_tileset(tileset_static_objects);
    signboard_.set_tile_idx(1);

    // tilemap
    tilemap_init(&tilemap_, 1, &map_002, &tileset_map_elements);
    tilemap_set_position(&tilemap_, MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(0));
    tilemap_set_r_cell_offset(&tilemap_, 1, 1);
    tilemap_set_enabled(&tilemap_, true);

    // tilemap (background)
    tilemap_init(&tilemap_bg_, 1, &map_002_bg, &tileset_map_elements);
    tilemap_set_position(&tilemap_bg_, MGC_CELL2PIXEL(-2), MGC_CELL2PIXEL(10));
    tilemap_set_r_cell_offset(&tilemap_bg_, 8, 8);
    tilemap_set_enabled(&tilemap_bg_, true);
}

}
