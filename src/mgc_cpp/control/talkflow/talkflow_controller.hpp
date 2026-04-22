/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_CONTROL_TALKFLOW_TALKFLOW_CONTROLLER_HPP
#define MGC_CONTROL_TALKFLOW_TALKFLOW_CONTROLLER_HPP

#include <type_traits>
#include "talkflow_listener.hpp"
#include "talkflow_effects.hpp"
#include "mgc_cpp/internal/common.hpp"
#include "mgc_cpp/features/resettable.hpp"
#include "mgc_cpp/features/drawable.hpp"
#include "mgc_cpp/features/cell_drawable.hpp"
#include "mgc_cpp/parts/assets/font.hpp"
#include "mgc_cpp/parts/assets/talkscript.hpp"
#include "mgc_cpp/platform/input/ibutton.hpp"
#include "mgc_cpp/parts/basic_selectbox.hpp"
#include "mgc_cpp/parts/basic_dialoguebox.hpp"

namespace mgc {
namespace control {
namespace talkflow {

template<typename SelectboxT, typename DialogueboxT>
struct TalkflowController : mgc::features::Resettable,
                            mgc::features::Drawable,
                            mgc::features::CellDrawable {

    static_assert(std::is_base_of<mgc::parts::interfaces::ISelectbox<SelectboxT, const char*>, SelectboxT>::value,
              "SelectboxT must inherit from ISelectbox<SelectboxT, const char*>");

    static_assert(std::is_base_of<mgc::parts::interfaces::IDialoguebox<DialogueboxT>, DialogueboxT>::value,
              "DialogueboxT must inherit from IDialoguebox<DialogueboxT>");

    explicit TalkflowController(mgc::platform::input::IButton &button) : button_(button) { reset(); }
    ~TalkflowController() = default;
    TalkflowController(const TalkflowController&) = delete;
    TalkflowController& operator=(const TalkflowController&) = delete;
    TalkflowController(TalkflowController&&) = default;
    TalkflowController& operator=(TalkflowController&&) = default;

    void bind_listener(ITalkflowListener& listener) {
        listener_ = &listener;
    }

    void unbind_listener() {
        listener_ = nullptr;
    }

    void bind_effects(ITalkflowEffects& effects) {
        effects_ = &effects;
    }

    void unbind_effects() {
        effects_ = nullptr;
    }

    void set_font(const mgc::parts::assets::Font& font) {
        selectbox_.set_font(font);
        dialoguebox_.set_font(font);
    }

    void set_talkscript(const mgc::parts::assets::TalkScript& talkscript) {
        talkflow_set_talkscript(&talkflow_, &talkscript);
    }

    void set_selectbox_config(const SelectboxConfig& config) {
        selectbox_.set_position(config.position);
        selectbox_.set_size(config.size);
        selectbox_.set_cursor_symbol(config.cursor_symbol);
        selectbox_.set_cursor_offset(config.cursor_offset);
    }

    void selectbox_config(SelectboxConfig& out) const {
        out.position = selectbox_.position();
        out.size = selectbox_.size();
        out.cursor_symbol = selectbox_.cursor_symbol();
        out.cursor_offset = selectbox_.cursor_offset();
    }

    void set_dialoguebox_config(const DialogueboxConfig& config) {
        dialoguebox_.set_position(config.position);
        dialoguebox_.set_size(config.size);
        dialoguebox_.set_padding(config.padding);
        dialoguebox_.set_scroll_speed(config.scroll_speed);
        dialoguebox_.set_scroll_threshold_line(config.scroll_threshold_line);
        dialoguebox_.set_line_spacing(config.line_spacing);
        dialoguebox_.set_typing_speed(config.typing_speed);
    }

    void dialoguebox_config(DialogueboxConfig& out) const {
        out.position = dialoguebox_.position();
        out.size = dialoguebox_.size();
        out.padding = dialoguebox_.padding();
        out.scroll_speed = dialoguebox_.scroll_speed();
        out.scroll_threshold_line = dialoguebox_.scroll_threshold_line();
        out.typing_speed = dialoguebox_.typing_speed();
    }

    void begin(mgc_node_idx_t entry_label = 0) {
        talkflow_begin(&talkflow_, entry_label);
    }

    void proc() {
        talkflow_proc(&talkflow_);
    }

    void proc(mgc::platform::input::IButton* temp_button) {
        mgc::platform::input::IButton* prev = active_button_;
        active_button_ = temp_button ? temp_button : &button_;

        talkflow_proc(&talkflow_);

        active_button_ = prev;
    }

    bool in_progress() const {
        auto state = talkflow_get_state(&talkflow_);
        
        return (
            ( state == MGC_TALKFLOW_STATE_SETUP ) ||
            ( state == MGC_TALKFLOW_STATE_PROCESSING )
        );
    }

    bool has_finished() const {
        auto state = talkflow_get_state(&talkflow_);
        return state == MGC_TALKFLOW_STATE_FLOW_END;
    }

    void reset_state() {
        talkflow_reset_state(&talkflow_);
    }

    const SelectboxT& selectbox() const {return selectbox_; }
    const DialogueboxT& dialoguebox() const {return dialoguebox_; }

    // [feature] Resettable
    void reset() override {
        
        active_button_ = &button_;

        selectbox_.reset();
        selectbox_.set_visible(false);

        dialoguebox_.reset();
        dialoguebox_.set_visible(false);

        talkflow_init(&talkflow_);
        bind_callbacks();
        listener_ = nullptr;
        effects_ = nullptr;
    }

    // [feature] Drawable
    using mgc::features::Drawable::draw;
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override {
        bool result = false;
        result |= dialoguebox_.draw(fb, cam_pos, options);
        result |= selectbox_.draw(fb, cam_pos, options);
        return result;
    }

    // [feature] CellDrawable
    using mgc::features::CellDrawable::cell_draw;
    bool cell_draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::math::Vec2i &cam_pos, const mgc::graphics::DrawOptions *options) const override {
        bool result = false;
        result |= dialoguebox_.cell_draw(cb, cell_x, cell_y, cam_pos, options);
        result |= selectbox_.cell_draw(cb, cell_x, cell_y, cam_pos, options);
        return result;
    }

private:
    mgc::platform::input::IButton& button_;
    mgc::platform::input::IButton* active_button_;
    ITalkflowListener *listener_;
    ITalkflowEffects *effects_;
    mgc_talkflow_t talkflow_;
    SelectboxT selectbox_;
    DialogueboxT dialoguebox_;
    mgc_talkflow_callbacks_t callbacks_;

    void bind_callbacks() {
        callbacks_ = mgc_talkflow_callbacks_t{
            this,
            on_setup_message_wrapper,
            on_setup_message_format_wrapper,
            on_setup_choice_wrapper,
            on_proc_message_wrapper,
            on_proc_choice_wrapper,
            on_decision_wrapper,
            on_select_wrapper,
            on_flow_end_wrapper
        };
        talkflow_set_callbacks(&talkflow_, &callbacks_);
    }

    static void on_setup_message_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const char *message_string, void *context) {
        static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_setup_message(talkflow, tag, message_string);
    }

    static void on_setup_message_format_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const char *format_string, void *context) {
        static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_setup_message_format(talkflow, tag, format_string);
    }

    static void on_setup_choice_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_choice_item_t *items, size_t item_count, void *context) {
        static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_setup_choice(talkflow, tag, items, item_count);
    }

    static enum mgc_talkflow_ui_state on_proc_message_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_message_t *message, void *context) {
        return static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_proc_message(talkflow, tag, message);
    }

    static enum mgc_talkflow_ui_state on_proc_choice_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_choice_t *choice, void *context) {
        return static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_proc_choice(talkflow, tag, choice);
    }

    static bool on_decision_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, void *context) {
        return static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_decision(talkflow, tag);
    }

    static int32_t on_select_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, void *context) {
        return static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_select(talkflow, tag);
    }

    static void on_flow_end_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_talknode_t *node, void *context) {
        static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_flow_end(talkflow, tag, node);
    }

    void on_setup_message( mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const char *message_string) {

        mgc::control::talkflow::DialogueboxConfig config;

        this->dialoguebox_config(config);
        if ( listener_ && listener_->on_start_message(tag, config) ) {
            this->set_dialoguebox_config(config);
        }

        dialoguebox_.set_text(message_string);
        dialoguebox_.set_visible(true);
    }

    void on_setup_message_format(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const char *format_string) {

        mgc::control::talkflow::DialogueboxConfig config;
        this->dialoguebox_config(config);
        if ( listener_ && listener_->on_start_message(tag, config) ) {
            this->set_dialoguebox_config(config);
        }

        if ( listener_ ) {
            const char * text = 
                listener_->on_get_message_format(tag, format_string);
            dialoguebox_.set_text(text);
        } else {
            dialoguebox_.set_text(format_string);
        }
        dialoguebox_.set_visible(true);
    }

    void on_setup_choice(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_choice_item_t *items, size_t item_count) {

        selectbox_.clear_items();
        for ( size_t i = 0; i < item_count; i++ ) {
            selectbox_.add_item(items[i].text);
        }
        selectbox_.set_selected_index(0);
        selectbox_.set_visible(true);
        selectbox_.adjust_size_to_fit();

        mgc::control::talkflow::SelectboxConfig config;
        this->selectbox_config(config);
        if ( listener_ && listener_->on_start_choice(tag, config) ) {
            this->set_selectbox_config(config);
        }
    }

    enum mgc_talkflow_ui_state on_proc_message(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_message_t *message) {
        
        MGC_ASSERT(active_button_ != nullptr, "active_button_ must not be nullptr");

        dialoguebox_.advance_typing();

        if ( dialoguebox_.is_typing_complete() ) {
            if ( active_button_->just_released(mgc::platform::input::Key::Enter) ||
                 message->auto_next
            ) {
                if ( listener_ ) {
                    listener_->on_message_done(tag);
                }
                return MGC_TALKFLOW_UI_STATE_FINISHED;
            }
        } else {
            if ( effects_ ) {
                effects_->play_typing_sound(tag, dialoguebox_.is_scrolling());
            }
        }
        return MGC_TALKFLOW_UI_STATE_CONTINUE;
    }

    enum mgc_talkflow_ui_state on_proc_choice(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_choice_t *choice) {

        MGC_ASSERT(active_button_ != nullptr, "active_button_ must not be nullptr");

        if ( active_button_->just_pressed(mgc::platform::input::Key::Up) ) {
            selectbox_.select_previous();
            if ( effects_ ) {
                effects_->play_select_move_sound(tag);
            }
        } else if ( active_button_->just_pressed(mgc::platform::input::Key::Down) ) {
            selectbox_.select_next();
            if ( effects_ ) {
                effects_->play_select_move_sound(tag);
            }
        } else if ( active_button_->just_released(mgc::platform::input::Key::Enter) ) {
            int32_t value = talkscript_get_item_value(choice, selectbox_.selected_index());
            if ( listener_ ) {
                listener_->on_choice_done(tag, selectbox_.selected_index(), value);
            }
            if ( effects_ ) {
                effects_->play_choice_confirm_sound(tag);
            }
            talkflow_decide_choice(talkflow, selectbox_.selected_index());
            selectbox_.set_visible(false);
            return MGC_TALKFLOW_UI_STATE_FINISHED;

        } else {
        }
        return MGC_TALKFLOW_UI_STATE_CONTINUE;
    }

    bool on_decision(mgc_talkflow_t *talkflow, mgc_node_idx_t tag) {
        (void)talkflow;
        if ( !listener_ ) {
            return false;
        }
        return listener_->on_decision(tag);
    }

    int32_t on_select(mgc_talkflow_t *talkflow, mgc_node_idx_t tag) {
        (void)talkflow;
        if ( !listener_ ) {
            return false;
        }
        return listener_->on_select(tag);
    }

    void on_flow_end(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_talknode_t *node) {
        (void)talkflow;
        if ( listener_ ) {
            listener_->on_flow_end(tag);
        }
        selectbox_.set_visible(false);
        dialoguebox_.set_visible(false);
    }
};

using DefaultTalkflowController = mgc::control::talkflow::TalkflowController<mgc::parts::BasicSelectbox, mgc::parts::BasicDialoguebox>;

}// namespace talkflow
}// namespace control
}// namespace mgc

#endif/*MGC_CONTROL_TALKFLOW_TALKFLOW_CONTROLLER_HPP*/
