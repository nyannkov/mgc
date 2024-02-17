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
    all_draw_flag_ = true;
    this->init_components();
}

void SceneSub::update() {
    mgc_sub_screen_ctrl_req request;
    auto gamepad = sys_get_gamepad_driver();
    auto sound = sys_get_sound_driver();
    if ( sub_screen_local_check_request(&request) ) {
        switch (request) {
        case MGC_SUB_SCREEN_CTRL_REQ_NONE:
            break;
        case MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_CURSOR_END:
            dialoguebox_set_text(&dialoguebox_, sub_screen_local_get_req_text());
            dialoguebox_set_enabled(&dialoguebox_, true);
            sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_10_WAIT_CURSOR_END);
            break;
        case MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_BUTTON_PRESSED:
            dialoguebox_set_text(&dialoguebox_, sub_screen_local_get_req_text());
            dialoguebox_set_enabled(&dialoguebox_, true);
            sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_20_WAIT_CURSOR_END);
            break;
        case MGC_SUB_SCREEN_CTRL_REQ_SHOW_TEXT_WAIT_RECEIVE_YESNO:
            dialoguebox_set_text(&dialoguebox_, sub_screen_local_get_req_text());
            dialoguebox_set_enabled(&dialoguebox_, true);
            sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_30_WAIT_CURSOR_END);
            break;
        case MGC_SUB_SCREEN_CTRL_REQ_CLOSE:
            dialoguebox_display_clear(&dialoguebox_);
            dialoguebox_set_enabled(&dialoguebox_, false);
            selectbox_set_enabled(&yesnobox_, false);
            sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_00_STOP);
            all_draw_flag_ = true;
            break;
        default:
            break;
        }
    } else {
        switch ( sub_screen_local_get_state() ) {
        case MGC_SUB_SCREEN_CTRL_10_WAIT_CURSOR_END:
            if ( dialoguebox_get_display_text_state(&dialoguebox_) ==
                 MGC_DISPLAY_TEXT_STATE_TEXT_END
            ) {
                sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_11_FINISHED);
            }
            break;
        case MGC_SUB_SCREEN_CTRL_20_WAIT_CURSOR_END:
            if ( dialoguebox_get_display_text_state(&dialoguebox_) ==
                 MGC_DISPLAY_TEXT_STATE_TEXT_END
            ) {
                sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_21_WAIT_BUTTON_PRESSED);
            }
            break;
        case MGC_SUB_SCREEN_CTRL_21_WAIT_BUTTON_PRESSED:
            if ( gamepad->is_key_on_edge(GP_KEY_1) ) {
                sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_22_FINISHED);
            }
            break;
        case MGC_SUB_SCREEN_CTRL_30_WAIT_CURSOR_END:
            if ( dialoguebox_get_display_text_state(&dialoguebox_) ==
                 MGC_DISPLAY_TEXT_STATE_TEXT_END
            ) {
                sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_31_WAIT_RESPONSE);
                selectbox_set_selected_idx(&yesnobox_, 0);
                selectbox_set_enabled(&yesnobox_, true);
            }
            break;
        case MGC_SUB_SCREEN_CTRL_31_WAIT_RESPONSE:
            if ( gamepad->is_key_on_edge(GP_KEY_1) ) {
                if ( yesnobox_.selected_idx == 0 ) {
                    sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_32_FINISHED_RECEIVED_YES);
                } else {
                    sub_screen_local_set_state(MGC_SUB_SCREEN_CTRL_33_FINISHED_RECEIVED_NO);
                }
                selectbox_set_enabled(&yesnobox_, false);
                sound->play_se(SE_ID_002);
            } else {
                if ( gamepad->get_firing_state(GP_KEY_UP) == GP_FIRING_ON ) {
                    selectbox_change_selected_idx(&yesnobox_, false);
                    sound->play_se(SE_ID_000);
                } else if ( gamepad->get_firing_state(GP_KEY_DOWN) == GP_FIRING_ON ) {
                    selectbox_change_selected_idx(&yesnobox_, true);
                    sound->play_se(SE_ID_000);
                } else {
                }
            }
            break;
        default:
            break;
        }
    }

    update_frame_rate();

    dialoguebox_display_update(&dialoguebox_);

    if ( dialoguebox_get_display_text_state(&dialoguebox_) ==
         MGC_DISPLAY_TEXT_STATE_CURSOR_MOVING
    ) {
        sound->play_se(SE_ID_001);
    }
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
            if ( selectbox_apply_cell_blending(&yesnobox_, &pixelbuffer_, x, y) ) {
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

    // yesnobox(selectbox)
    selectbox_init(&yesnobox_, 0, sys_get_default_font(), false);
    selectbox_set_position(&yesnobox_, 8, 40);
    selectbox_append_item(&yesnobox_, "はい");
    selectbox_append_item(&yesnobox_, "いいえ");
    selectbox_set_width(&yesnobox_, 8*6);
    selectbox_set_enabled(&yesnobox_, false);
}

void SceneSub::update_frame_rate() {
    snprintf(str_frame_rate_, sizeof(str_frame_rate_)-1, "Frame rate : %.1f fps", sys_get_frame_rate());
    label_set_text(&label_frame_rate_, str_frame_rate_);
}

}
