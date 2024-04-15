/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include <cstdio>
#include "system/system.h"
#include "scene_sub.hpp"
#include "screen/sub/sub_screen_local.h"

namespace mgc::sub_screen {

void SceneSub::init() {
    init_dlgflow_handler();
    all_draw_flag_ = true;
    this->init_components();
}

void SceneSub::update() {
    const auto gamepad = sys_get_gamepad_driver();
    const auto sound = sys_get_sound_driver()->common_if;

    if ( check_request_show_dialoguebox() ) {
        dialoguebox_set_text(&dialoguebox_, get_show_dialoguebox_text());
        dialoguebox_set_enabled(&dialoguebox_, true);
        proc_dialoguebox_flag_ = true;
    }

    if ( check_request_close_dialoguebox() ) {
        dialoguebox_display_clear(&dialoguebox_);
        dialoguebox_set_enabled(&dialoguebox_, false);
        all_draw_flag_ = true;
    }

    if ( check_request_show_selectbox() ) {
        const char **item_array = get_show_selectbox_item_array();
        size_t item_count = get_show_selectbox_item_count();
        selectbox_clear_items(&selectbox_);
        for ( size_t idx = 0; idx < item_count; idx++ ) {
            selectbox_append_item(&selectbox_, item_array[idx]);
        }
        selectbox_set_selected_idx(&selectbox_, 0);
        selectbox_set_enabled(&selectbox_, true);
        proc_selectbox_flag_ = true;
    }

    if ( check_request_close_selectbox() ) {
        selectbox_set_enabled(&selectbox_, false);
        all_draw_flag_ = true;
    }

    if ( proc_dialoguebox_flag_ ) {
        enum mgc_display_text_state state;
        dialoguebox_display_update(&dialoguebox_);
        state = dialoguebox_get_display_text_state(&dialoguebox_);
        if ( state == MGC_DISPLAY_TEXT_STATE_TEXT_END ) {
            uint32_t flags = get_show_dialoguebox_flags();
            if ( ( gamepad->is_key_on_edge(GP_KEY_1) ) ||
                 ( ( flags & 0x01 ) != 0 )
             ) {
                set_proc_dialoguebox_result(0);
                proc_dialoguebox_flag_ = false;
            }
        } else if ( state == MGC_DISPLAY_TEXT_STATE_CURSOR_MOVING ) {
            sound->play_se(SE_ID_001);
        } else {
        }
    }

    if ( proc_selectbox_flag_ ) {
        if ( gamepad->get_firing_state(GP_KEY_UP) ) {
            selectbox_change_selected_idx(&selectbox_, false);
            sound->play_se(SE_ID_001);
        }
        if ( gamepad->get_firing_state(GP_KEY_DOWN) ) {
            selectbox_change_selected_idx(&selectbox_, true);
            sound->play_se(SE_ID_001);
        }
        if ( gamepad->is_key_on_edge(GP_KEY_1) ) {
            set_proc_selectbox_result(selectbox_.selected_idx);
            proc_selectbox_flag_ = false;
            sound->play_se(SE_ID_002);
        }
    }

    update_frame_rate();
}

void SceneSub::draw(uint16_t panel_x0, uint16_t panel_y0, uint16_t width, uint16_t height) {
    for ( int16_t y = 0; y < height; y += MGC_CELL2PIXEL(1)) {
        for ( int16_t x = 0; x < width; x += MGC_CELL2PIXEL(1)) {
            bool is_blending = false;
            if ( label_apply_cell_blending(&label_frame_rate_, &pixelbuffer_, x, y) ) {
                is_blending = true;
            }
            if ( dialoguebox_apply_cell_blending(&dialoguebox_, &pixelbuffer_, x, y) ) {
                is_blending = true;
            }
            if ( selectbox_apply_cell_blending(&selectbox_, &pixelbuffer_, x, y) ) {
                is_blending = true;
            }
            if ( is_blending || all_draw_flag_ ) {
                pixelbuffer_draw_cell(&pixelbuffer_, sys_get_display_driver(), panel_x0+x, panel_y0+y);
            }
        }
    }
    all_draw_flag_ = false;
}
void SceneSub::init_components() {
    // pixelbuffer
    pixelbuffer_init(&pixelbuffer_);

    // label_frame_rate_
    label_init(&label_frame_rate_, 0, sys_get_default_font(), false);
    label_set_size(&label_frame_rate_, 8*32, 12);
    label_set_position(&label_frame_rate_, MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(0));
    label_set_enabled(&label_frame_rate_, true);
    update_frame_rate();

    // dialoguebox
    dialoguebox_init(&dialoguebox_, 0, sys_get_default_font(), false);
    dialoguebox_set_position(&dialoguebox_, 8, 8);
    dialoguebox_set_width(&dialoguebox_, 224);
    dialoguebox_set_height(&dialoguebox_, 12*4);
    dialoguebox_set_cursor_speed(&dialoguebox_, 4);
    dialoguebox_set_scroll_speed(&dialoguebox_, 6);
    dialoguebox_set_scroll_line(&dialoguebox_, 3);
    dialoguebox_set_line_spacing(&dialoguebox_, 8);
    dialoguebox_adjust_height(&dialoguebox_);
    dialoguebox_set_text(&dialoguebox_, "");
    dialoguebox_set_enabled(&dialoguebox_, false);

    // selectbox
    selectbox_init(&selectbox_, 0, sys_get_default_font(), false);
    selectbox_set_position(&selectbox_, 8, 40);
    selectbox_append_item(&selectbox_, "はい");
    selectbox_append_item(&selectbox_, "いいえ");
    selectbox_set_width(&selectbox_, 8*6);
    selectbox_set_enabled(&selectbox_, false);
}

void SceneSub::update_frame_rate() {
    snprintf(str_frame_rate_, sizeof(str_frame_rate_)-1, "Frame rate : %.1f fps", sys_get_frame_rate());
    label_set_text(&label_frame_rate_, str_frame_rate_);
}

}
