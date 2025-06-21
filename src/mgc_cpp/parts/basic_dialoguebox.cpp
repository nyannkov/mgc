/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "basic_dialoguebox.hpp"

namespace mgc {
namespace parts {

using mgc::math::Vec2i;
using mgc::parts::types::Padding;
using mgc::parts::types::Size;
using mgc::parts::assets::Font;
using mgc::graphics::Color;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;
using mgc::graphics::DrawOptions;
using mgc::graphics::ParallaxFactor;

// [feature] Hitbox
void mgc::parts::BasicDialoguebox::reset() {
    dialoguebox_init(&dialoguebox_, 0, nullptr, false);
}

// [feature] HasId
void mgc::parts::BasicDialoguebox::set_id(mgc_id_t id) {
    dialoguebox_set_id(&dialoguebox_, id);
}

mgc_id_t mgc::parts::BasicDialoguebox::id() const {
    return dialoguebox_get_id(&dialoguebox_);
}

// [feature] HasPosition
Vec2i mgc::parts::BasicDialoguebox::position() const {
    mgc_point_t point = dialoguebox_get_position(&dialoguebox_);
    return Vec2i(point.x, point.y);
}

void mgc::parts::BasicDialoguebox::set_position(const Vec2i& position) {
    dialoguebox_set_position(&dialoguebox_, position.x, position.y);
}

// [feature] HasParallaxFactor
void mgc::parts::BasicDialoguebox::set_parallax_factor(const ParallaxFactor &factor) {
    auto f = factor.to_c();
    dialoguebox_set_parallax_factor(&dialoguebox_, f.f_x, f.f_y);
}

ParallaxFactor mgc::parts::BasicDialoguebox::parallax_factor() const {
    return ParallaxFactor(dialoguebox_get_parallax_factor(&dialoguebox_));
}

// [feature] Visible
bool mgc::parts::BasicDialoguebox::is_visible() const {
    return dialoguebox_get_visible(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_visible(bool v) {
    dialoguebox_set_visible(&dialoguebox_, v);
}

// [feature] Drawable
bool mgc::parts::BasicDialoguebox::draw(Framebuffer &fb, const Vec2i &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = mgc_point_t{cam_pos.x, cam_pos.y};
    return dialoguebox_draw_raw(&dialoguebox_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
bool mgc::parts::BasicDialoguebox::cell_draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Vec2i &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = mgc_point_t{cam_pos.x, cam_pos.y};
    return dialoguebox_draw_cell_raw(&dialoguebox_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithSize
Size mgc::parts::BasicDialoguebox::size_impl() const {
    return Size(dialoguebox_get_size(&dialoguebox_));
}

void mgc::parts::BasicDialoguebox::set_size_impl(uint16_t width, uint16_t height) {
    dialoguebox_set_width(&dialoguebox_, width);
    dialoguebox_set_height(&dialoguebox_, height);
}

//// WithSize-related
void mgc::parts::BasicDialoguebox::adjust_size_to_fit() {
    dialoguebox_adjust_height(&dialoguebox_);
}

// [impl] WithPadding
void mgc::parts::BasicDialoguebox::set_padding_impl(Padding padding) {
    auto pad = padding.to_c();
    dialoguebox_set_padding(
            &dialoguebox_,
            pad.top,
            pad.bottom,
            pad.left,
            pad.right
    );
}

Padding mgc::parts::BasicDialoguebox::padding_impl() const {
    return Padding(dialoguebox_get_padding(&dialoguebox_));
}

// [impl] WithFont
void mgc::parts::BasicDialoguebox::set_font_impl(const Font& font) {
    dialoguebox_set_font(&dialoguebox_, &font);
}

const Font* mgc::parts::BasicDialoguebox::font_impl() const {
    return dialoguebox_get_font(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_font_fore_color_impl(Color color) {
    dialoguebox_set_fore_color(&dialoguebox_, color);
}

Color mgc::parts::BasicDialoguebox::font_fore_color_impl() const {
    return dialoguebox_get_fore_color(&dialoguebox_);
}

//// WithFont-related
void mgc::parts::BasicDialoguebox::set_fontsize2x(bool fontsize2x) {
    dialoguebox_set_fontsize2x(&dialoguebox_, fontsize2x);
}

Color mgc::parts::BasicDialoguebox::back_color() const {
    return dialoguebox_get_back_color(&dialoguebox_);
}

bool mgc::parts::BasicDialoguebox::is_back_enabled() const {
    return dialoguebox_get_enable_back_color(&dialoguebox_);
}

// [impl] WithText
void mgc::parts::BasicDialoguebox::set_text_impl(const char *text) {
    dialoguebox_set_text(&dialoguebox_, text);
}

const char* mgc::parts::BasicDialoguebox::text_impl() const {
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

uint32_t mgc::parts::BasicDialoguebox::scroll_speed_impl() const {
    return dialoguebox_get_scroll_speed(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_scroll_threshold_line_impl(uint32_t threshold_line) {
    if ( threshold_line > UINT8_MAX ) {
        threshold_line = UINT8_MAX;
    }
    dialoguebox_set_scroll_line(&dialoguebox_, threshold_line);
}

uint32_t mgc::parts::BasicDialoguebox::scroll_threshold_line_impl() const {
    return dialoguebox_get_scroll_line(&dialoguebox_);
}

void mgc::parts::BasicDialoguebox::set_line_spacing_impl(uint32_t line_spacing) {
    if ( line_spacing > UINT8_MAX ) {
        line_spacing = UINT8_MAX;
    }
    dialoguebox_set_line_spacing(&dialoguebox_, line_spacing);
}

uint32_t mgc::parts::BasicDialoguebox::line_spacing_impl() const {
    return dialoguebox_get_line_spacing(&dialoguebox_);
}

// [impl] WithTypingEffect
void mgc::parts::BasicDialoguebox::set_typing_speed_impl(uint32_t chars_per_frame) {
    if ( chars_per_frame > UINT8_MAX ) {
        chars_per_frame = UINT8_MAX;
    }
    dialoguebox_set_cursor_speed(&dialoguebox_, chars_per_frame);
}

uint32_t mgc::parts::BasicDialoguebox::typing_speed_impl() const {
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

