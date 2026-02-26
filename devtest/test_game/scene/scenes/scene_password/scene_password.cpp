#include "scene_password.hpp"
#include "resources/generated/font/k8x12.h"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/mml/mml.h"
#include <stdio.h>

namespace app {

using mgc::platform::input::Key;

Scene_Password::Scene_Password(GameContext& ctx)
                : SceneBase(ctx),
                  world_state_(ctx.world_state) {

    set_id(SceneId::Password);
}

void Scene_Password::init() {
    label_button_.set_position({32, 64});
    label_button_.set_size({12*8, 12});
    label_button_.set_font(k8x12);
    label_button_.set_fontsize2x(false);
    label_button_.set_font_fore_color(MGC_COLOR_BLACK);
    label_button_.set_back_color(MGC_COLOR_WHITE);
    label_button_.set_enabled_back_color(true);

    clear_input_text();
    diag_input_.set_position({48, 16});
    diag_input_.set_size({128, 32});
    diag_input_.set_typing_speed(0);
    diag_input_.set_font(k8x12);
    diag_input_.set_fontsize2x(false);
    diag_input_.set_font_fore_color(MGC_COLOR_WHITE);
    diag_input_.set_text(input_buffer_);

    talkflow_.bind_listener(talkflow_listener_);
    talkflow_.set_talkscript(talkscript_1);

    status_display_request_.request_hide();
    scx_.sound.stop_background_music(0.0);
}

void Scene_Password::update() {

    talkflow_.proc();
    if ( talkflow_.has_finished() ) {
        talkflow_.reset_state();
    }

    if ( !talkflow_.in_progress() ) {
        if ( input_state_ == InputState::ConfirmExit ) {
            if ( talkflow_listener_.is_choice_done() ) {
                if ( talkflow_listener_.choice_item_tag() == 
                    MGC_TALKSCRIPT_1_TAG_CHOICE__CONFIRM_PASSWORD_EXIT__ITEM__YES
                ) {
                    set_scene_change_request(SceneId::Title);
                } else {
                    talkflow_listener_.clear();
                    input_state_ = InputState::None;
                }
            }
        } else {
            update_input();
        }
    }
}

void Scene_Password::update_input() {
    if ( scx_.gamepad.just_pressed(Key::Up) ) {
        if ( 0 <= selected_index_ && selected_index_ <= 5 ) {
            selected_index_ = BUTTON_INDEX_OK;
        } else if ( 6 <= selected_index_ && selected_index_ <= 9 ) {
            selected_index_ = BUTTON_INDEX_BACK;
        } else if ( selected_index_ == BUTTON_INDEX_OK ) {
            selected_index_ = 32;
        } else if ( selected_index_ == BUTTON_INDEX_BACK ) {
            selected_index_ = 26;
        } else {
            selected_index_ -= 10;
        }
    } else if ( scx_.gamepad.just_pressed(Key::Down) ) {
        if ( (26 <= selected_index_ && selected_index_ <= 29) || 
             (selected_index_ == 35)
        ) {
            selected_index_ = BUTTON_INDEX_BACK;
        } else if ( 30 <= selected_index_ && selected_index_ <= 34 ) {
            selected_index_ = BUTTON_INDEX_OK;
        } else if ( selected_index_ == BUTTON_INDEX_OK ) {
            selected_index_ = 2;
        } else if ( selected_index_ == BUTTON_INDEX_BACK ) {
            selected_index_ = 6;
        } else {
            selected_index_ += 10;
        }
    } else if ( scx_.gamepad.just_pressed(Key::Right) ) {
        if ( selected_index_ < countof(BUTTON_TEXT)-1 ) {
            selected_index_++;
        } else {
            selected_index_ = 0;
        }
    } else if ( scx_.gamepad.just_pressed(Key::Left) ) {
        if ( 0 < selected_index_ ) {
            selected_index_--;
        } else {
            selected_index_ = countof(BUTTON_TEXT)-1;
        }
    }

    if ( scx_.gamepad.just_pressed(Key::Enter) ) {
        if ( 0 <= selected_index_ && selected_index_ < 36 ) {
            add_input_text(BUTTON_TEXT[selected_index_][0]);
            diag_input_.set_text(input_buffer_);
            if ( is_input_text_full() ) {
                selected_index_ = BUTTON_INDEX_OK;
            }
        } else if ( selected_index_ == BUTTON_INDEX_OK ) {
            SaveData save_data{};
            bool r = SaveDataEncoder::decode(enc_data_, save_data);
            if ( r ) {
                talkflow_.begin(MGC_TALKSCRIPT_1_PASSWORD_SUCCESS);
            } else {
                talkflow_.begin(MGC_TALKSCRIPT_1_PASSWORD_FAILURE);
            }
        } else if ( selected_index_ == BUTTON_INDEX_BACK ) {
            talkflow_.begin(MGC_TALKSCRIPT_1_PASSWORD_EXIT);
            input_state_ = InputState::ConfirmExit;
        } else { }
    } else if ( scx_.gamepad.just_pressed(Key::Cancel) ) {
        backspace_input_text();
    } else { }

    diag_input_.advance_typing();
}

void Scene_Password::draw(mgc::graphics::Framebuffer& fb) {
    fb.clear(MGC_COLOR_BLACK);
    size_t char_set_index = 0;
    for ( size_t i = 0; i < 4; ++i ) {
        for ( size_t j = 0; j < 10; ++j ) {
            if ( char_set_index < 36 ) {
                mgc::math::Vec2i pos{};
                pos.x = j * 16 + 32;
                pos.y = i * 16 + 64;
                label_button_.set_position(pos);
                label_button_.set_text(BUTTON_TEXT[char_set_index]);
                draw_button(fb, label_button_, char_set_index);
                char_set_index++;
            } else {
                break;
            }
        }
    }

    label_button_.set_position({16*4, 136});
    label_button_.set_text(BUTTON_TEXT[BUTTON_INDEX_OK]);
    draw_button(fb, label_button_, BUTTON_INDEX_OK);

    label_button_.set_position({16*8, 136});
    label_button_.set_text(BUTTON_TEXT[BUTTON_INDEX_BACK]);
    draw_button(fb, label_button_, BUTTON_INDEX_BACK);

    diag_input_.draw(fb);    

    talkflow_.draw(fb);
}

void Scene_Password::clear_input_text() {
    cursor_input_text_ = 0;
    input_buffer_[8] = input_buffer_[18] = '-';
    input_buffer_[9] = input_buffer_[19] = ' ';

    input_buffer_[0]  = input_buffer_[2]  = input_buffer_[4]  = input_buffer_[6]  =
    input_buffer_[10] = input_buffer_[12] = input_buffer_[14] = input_buffer_[16] =
    input_buffer_[20] = input_buffer_[22] = input_buffer_[24] = input_buffer_[26] = '_';
    input_buffer_[1]  = input_buffer_[3]  = input_buffer_[5]  = input_buffer_[7]  =
    input_buffer_[11] = input_buffer_[13] = input_buffer_[15] = input_buffer_[17] =
    input_buffer_[21] = input_buffer_[23] = input_buffer_[25] = input_buffer_[27] = ' ';
    input_buffer_[28] = '\0';
}

void Scene_Password::add_input_text(char c) {
    if ( !is_input_text_full() ) {
        enc_data_.data[cursor_input_text_] = c;

        auto *buf = get_cursor_buffer(cursor_input_text_);
        if ( buf ) {
            *buf = c;
        }
        cursor_input_text_++;
    }
}

void Scene_Password::backspace_input_text() {
    if ( 0 < cursor_input_text_ ) {
        cursor_input_text_--;
    }
    auto *buf = get_cursor_buffer(cursor_input_text_);
    if ( buf ) {
        *buf = '_';
    }
}

bool Scene_Password::is_input_text_full() const {
    return cursor_input_text_ >= 12;
}
char* Scene_Password::get_cursor_buffer(size_t cursor) {
    if ( 0 <= cursor && cursor <= 3 ) {
        return &input_buffer_[cursor*2];
    } else if ( 4 <= cursor && cursor <= 7 ) {
        return &input_buffer_[10 + (cursor-4)*2];
    } else if ( 8 <= cursor && cursor <= 11 ) {
        return &input_buffer_[20 + (cursor-8)*2];
    } else { 
        return nullptr;
    }
}
void Scene_Password::draw_button(FramebufferT& fb, LabelT& label, size_t button_index) {
    if ( button_index == selected_index_ ) {
        label.set_font_fore_color(MGC_COLOR_BLACK);
        label.set_back_color(MGC_COLOR_WHITE);
    } else {
        label.set_font_fore_color(MGC_COLOR_WHITE);
        label.set_back_color(MGC_COLOR_BLACK);
    }
    label.draw(fb);
}

} // namespace app

