/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "basic_dialoguebox.hpp"

namespace mgc {
namespace parts {

using mgc::geometry::Point;
using mgc::parts::types::ParallaxFactor;
using mgc::parts::types::DrawOptions;
using mgc::parts::types::Margin;
using mgc::parts::types::Size;
using mgc::parts::assets::Font;
using mgc::graphics::Color;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;

void mgc::parts::BasicDialoguebox::reset() {
    dialoguebox_init(&dialoguebox_, 0, nullptr, false);
}

// [feature] HasId
void mgc::parts::BasicDialoguebox::set_id(mgc_id_t id) {
    dialoguebox_set_id(&dialoguebox_, id);
}

mgc_id_t mgc::parts::BasicDialoguebox::get_id() const {
    return dialoguebox_get_id(&dialoguebox_);
}

// [feature] Positionable
Point mgc::parts::BasicDialoguebox::get_position() const {
    mgc_point_t point = dialoguebox_get_position(&dialoguebox_);
    return Point::from_c(point);
}

void mgc::parts::BasicDialoguebox::set_position(int16_t x, int16_t y) {
    dialoguebox_set_position(&dialoguebox_, x, y);
}

// [feature] HasParallaxFactor
void mgc::parts::BasicDialoguebox::set_parallax_factor(const ParallaxFactor &factor) {
    dialoguebox_set_parallax_factor(&dialoguebox_, factor.f_x, factor.f_y);
}

ParallaxFactor mgc::parts::BasicDialoguebox::get_parallax_factor() const {
    return dialoguebox_get_parallax_factor(&dialoguebox_);
}

// [feature] Visible
bool mgc::parts::BasicDialoguebox::is_visible() const {
    return dialoguebox_get_visible(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_visible(bool v) {
    dialoguebox_set_visible(&dialoguebox_, v);
}

// [feature] Drawable
bool mgc::parts::BasicDialoguebox::draw(Framebuffer &fb, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return dialoguebox_draw_raw(&dialoguebox_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
bool mgc::parts::BasicDialoguebox::draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return dialoguebox_draw_cell_raw(&dialoguebox_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithSize
Size mgc::parts::BasicDialoguebox::get_size_impl() const {
    return dialoguebox_get_size(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_size_impl(uint16_t width, uint16_t height) {
    dialoguebox_set_width(&dialoguebox_, width);
    dialoguebox_set_height(&dialoguebox_, height);
}
//// WithSize-related
void mgc::parts::BasicDialoguebox::adjust_size_to_fit() {
    dialoguebox_adjust_height(&dialoguebox_);
}

// [impl] WithMargin
void mgc::parts::BasicDialoguebox::set_margin_impl(Margin margin) {
    dialoguebox_set_margin(
            &dialoguebox_,
            margin.top,
            margin.bottom,
            margin.left,
            margin.right
    );
}

Margin mgc::parts::BasicDialoguebox::get_margin_impl() const {
    return dialoguebox_get_margin(&dialoguebox_);
}

// [impl] WithFont
void mgc::parts::BasicDialoguebox::set_font_impl(const Font& font) {
    dialoguebox_set_font(&dialoguebox_, &font);
}

const Font* mgc::parts::BasicDialoguebox::get_font_impl() const {
    return dialoguebox_get_font(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_fore_color_impl(Color color) {
    dialoguebox_set_fore_color(&dialoguebox_, color);
}

Color mgc::parts::BasicDialoguebox::get_fore_color_impl() const {
    return dialoguebox_get_fore_color(&dialoguebox_);
}

//// WithFont-related
void mgc::parts::BasicDialoguebox::set_fontsize2x(bool fontsize2x) {
    dialoguebox_set_fontsize2x(&dialoguebox_, fontsize2x);
}

Color mgc::parts::BasicDialoguebox::get_back_color() const {
    return dialoguebox_get_back_color(&dialoguebox_);
}

bool mgc::parts::BasicDialoguebox::is_back_enabled() const {
    return dialoguebox_get_enable_back_color(&dialoguebox_);
}

// [impl] WithText
void mgc::parts::BasicDialoguebox::set_text_impl(const char *text) {
    dialoguebox_set_text(&dialoguebox_, text);
}

const char* mgc::parts::BasicDialoguebox::get_text_impl() const {
    return dialoguebox_get_text(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::clear_text_impl() {
    dialoguebox_display_clear(&dialoguebox_);
}

// [impl] WithAutoTextScroll
void mgc::parts::BasicDialoguebox::set_scroll_speed_impl(uint32_t speed) {
    if ( speed > UINT8_MAX ) {
        speed = UINT8_MAX;
    }
    dialoguebox_set_scroll_speed(&dialoguebox_, speed);
}

uint32_t mgc::parts::BasicDialoguebox::get_scroll_speed_impl() const {
    return dialoguebox_get_scroll_speed(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_scroll_threshold_line_impl(uint32_t threshold_line) {
    if ( threshold_line > UINT8_MAX ) {
        threshold_line = UINT8_MAX;
    }
    dialoguebox_set_scroll_line(&dialoguebox_, threshold_line);
}

uint32_t mgc::parts::BasicDialoguebox::get_scroll_threshold_line_impl() const {
    return dialoguebox_get_scroll_line(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_line_spacing_impl(uint32_t line_spacing) {
    if ( line_spacing > UINT8_MAX ) {
        line_spacing = UINT8_MAX;
    }
    dialoguebox_set_line_spacing(&dialoguebox_, line_spacing);
}

uint32_t mgc::parts::BasicDialoguebox::get_line_spacing_impl() const {
    return dialoguebox_get_line_spacing(&dialoguebox_);
}

// [impl] WithTypingEffect
void mgc::parts::BasicDialoguebox::set_typing_speed_impl(uint32_t chars_per_frame) {
    if ( chars_per_frame > UINT8_MAX ) {
        chars_per_frame = UINT8_MAX;
    }
    dialoguebox_set_cursor_speed(&dialoguebox_, chars_per_frame);
}

uint32_t mgc::parts::BasicDialoguebox::get_typing_speed_impl() const {
    return dialoguebox_get_cursor_speed(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::advance_typing_impl() {
    dialoguebox_display_update(&dialoguebox_);
}

bool mgc::parts::BasicDialoguebox::is_typing_complete_impl() const {
    auto state = dialoguebox_get_display_text_state(&dialoguebox_);
    return state == MGC_DISPLAY_TEXT_STATE_TEXT_END;
}

}// namespace parts
}// namespace mgc

