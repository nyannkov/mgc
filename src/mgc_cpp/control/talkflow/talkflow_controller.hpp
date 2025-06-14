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

    void bind_listener(ITalkflowListener& listener) {
        listener_ = &listener;
    }

    void unbind_listener() {
        listener_ = nullptr;
    }

    void set_font(const mgc::parts::assets::Font& font) {
        selectbox_.set_font(font);
        dialoguebox_.set_font(font);
    }

    void set_talkscript(const mgc::parts::assets::TalkScript& talkscript) {
        talkflow_set_talkscript(&talkflow_, &talkscript);
    }

    void set_selectbox_config(const SelectboxConfig& config) {
        selectbox_.set_position(config.point);
        selectbox_.set_size(config.size);
        selectbox_.set_cursor_symbol(config.cursor_symbol);
        selectbox_.set_cursor_offset(config.cursor_offset);
    }

    void get_selectbox_config(SelectboxConfig& out) const {
        out.point = selectbox_.get_position();
        out.size = selectbox_.get_size();
        out.cursor_symbol = selectbox_.get_cursor_symbol();
        out.cursor_offset = selectbox_.get_cursor_offset();
    }

    void set_dialoguebox_config(const DialogueboxConfig& config) {
        dialoguebox_.set_position(config.point);
        dialoguebox_.set_size(config.size);
        dialoguebox_.set_scroll_speed(config.scroll_speed);
        dialoguebox_.set_scroll_threshold_line(config.scroll_threshold_line);
        dialoguebox_.set_line_spacing(config.line_spacing);
        dialoguebox_.set_typing_speed(config.typing_speed);
    }

    void get_dialoguebox_config(DialogueboxConfig& out) const {
        out.point = dialoguebox_.get_position();
        out.size = dialoguebox_.get_size();
        out.scroll_speed = dialoguebox_.get_scroll_speed();
        out.scroll_threshold_line = dialoguebox_.get_scroll_threshold_line();
        out.typing_speed = dialoguebox_.get_typing_speed();
    }

    void begin(mgc_node_idx_t entry_label = 0) {
        talkflow_begin(&talkflow_, entry_label);
    }

    void proc() {
        talkflow_proc(&talkflow_);
    }

    const SelectboxT& selectbox() const {return selectbox_; }
    const DialogueboxT& dialoguebox() const {return dialoguebox_; }

    // [feature] Resettable
    void reset() {
        selectbox_.reset();
        selectbox_.set_visible(false);

        dialoguebox_.reset();
        dialoguebox_.set_visible(false);

        talkflow_init(&talkflow_);
        bind_callbacks();
        listener_ = nullptr;
    }

    // [feature] Drawable
    bool draw(mgc::graphics::Framebuffer &fb, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const override {
        bool result = false;
        result |= dialoguebox_.draw(fb, cam_pos, options);
        result |= selectbox_.draw(fb, cam_pos, options);
        return result;
    }

    // [feature] CellDrawable
    bool draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const override {
        bool result = false;
        result |= dialoguebox_.draw(cb, cell_x, cell_y, cam_pos, options);
        result |= selectbox_.draw(cb, cell_x, cell_y, cam_pos, options);
        return result;
    }

private:
    mgc::platform::input::IButton& button_;
    ITalkflowListener *listener_;
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

    static void on_flow_end_wrapper(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_talknode_t *node, void *context) {
        static_cast<TalkflowController<SelectboxT, DialogueboxT>*>(context)->on_flow_end(talkflow, tag, node);
    }

    void on_setup_message( mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const char *message_string) {
        if ( !listener_ ) {
            return;
        }
        mgc::control::talkflow::DialogueboxConfig config;

        this->get_dialoguebox_config(config);
        if ( listener_->on_start_message(tag, config) ) {
            this->set_dialoguebox_config(config);
        }

        dialoguebox_.set_text(message_string);
        dialoguebox_.set_visible(true);
    }

    void on_setup_message_format(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const char *format_string) {
        if ( !listener_ ) {
            return;
        }

        mgc::control::talkflow::DialogueboxConfig config;
        this->get_dialoguebox_config(config);
        if ( listener_->on_start_message(tag, config) ) {
            this->set_dialoguebox_config(config);
        }

        const char * text = 
            listener_->on_get_message_format(tag, format_string);
        dialoguebox_.set_text(text);
        dialoguebox_.set_visible(true);
    }

    void on_setup_choice(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_choice_item_t *items, size_t item_count) {
        if ( !listener_ ) {
            return;
        }

        selectbox_.clear_items();
        for ( size_t i = 0; i < item_count; i++ ) {
            selectbox_.add_item(items[i].text);
        }
        selectbox_.set_selected_index(0);
        selectbox_.set_visible(true);
        selectbox_.adjust_size_to_fit();

        mgc::control::talkflow::SelectboxConfig config;
        this->get_selectbox_config(config);
        if ( listener_->on_start_choice(tag, config) ) {
            this->set_selectbox_config(config);
        }
    }

    enum mgc_talkflow_ui_state on_proc_message(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_message_t *message) {
        if ( !listener_ ) {
            return MGC_TALKFLOW_UI_STATE_FINISHED;
        }

        dialoguebox_.advance_typing();

        if ( dialoguebox_.is_typing_complete() ) {
            if ( button_.just_pressed(mgc::platform::input::Key::Enter) ) {
                listener_->on_message_done(tag);
                return MGC_TALKFLOW_UI_STATE_FINISHED;
            }
        }
        return MGC_TALKFLOW_UI_STATE_CONTINUE;
    }

    enum mgc_talkflow_ui_state on_proc_choice(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_node_choice_t *choice) {
        if ( !listener_ ) {
            return MGC_TALKFLOW_UI_STATE_FINISHED;
        }
        if ( button_.just_pressed(mgc::platform::input::Key::Up) ) {
            selectbox_.select_previous();

        } else if ( button_.just_pressed(mgc::platform::input::Key::Down) ) {
            selectbox_.select_next();

        } else if ( button_.just_pressed(mgc::platform::input::Key::Enter) ) {
            int32_t value = talkscript_get_item_value(choice, selectbox_.get_selected_index());
            listener_->on_choice_done(tag, selectbox_.get_selected_index(), value);
            talkflow_decide_choice(talkflow, selectbox_.get_selected_index());
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

    void on_flow_end(mgc_talkflow_t *talkflow, mgc_node_idx_t tag, const mgc_talknode_t *node) {
        (void)talkflow;
        if ( !listener_ ) {
            return;
        }

        listener_->on_flow_end(tag);
    }
};

using DefaultTalkflowController = mgc::control::talkflow::TalkflowController<mgc::parts::BasicSelectbox, mgc::parts::BasicDialoguebox>;

}// namespace talkflow
}// namespace control
}// namespace mgc

#endif/*MGC_CONTROL_TALKFLOW_TALKFLOW_CONTROLLER_HPP*/
