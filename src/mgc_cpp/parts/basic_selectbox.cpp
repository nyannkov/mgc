/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "basic_selectbox.hpp"

namespace mgc {
namespace parts {

using mgc::geometry::Point;
using mgc::parts::types::ParallaxFactor;
using mgc::parts::types::DrawOptions;
using mgc::parts::types::Padding;
using mgc::parts::types::Size;
using mgc::parts::assets::Font;
using mgc::graphics::Color;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;


// [feature] Resettable
void mgc::parts::BasicSelectbox::reset() {
    selectbox_init(&selectbox_, 0, nullptr, false);
}

// [feature] HasId
void mgc::parts::BasicSelectbox::set_id(mgc_id_t id) {
    selectbox_get_id(&selectbox_);
}

mgc_id_t mgc::parts::BasicSelectbox::get_id() const {
    return selectbox_get_id(&selectbox_);
}

// [feature] Positionable
Point mgc::parts::BasicSelectbox::get_position() const {
    mgc_point_t point = selectbox_get_position(&selectbox_);
    return Point::from_c(point);
}

void mgc::parts::BasicSelectbox::set_position(int16_t x, int16_t y) {
    selectbox_set_position(&selectbox_, x, y);
}

// [feature] HasParallaxFactor
void mgc::parts::BasicSelectbox::set_parallax_factor(const ParallaxFactor &factor) {
    selectbox_set_parallax_factor(&selectbox_, factor.f_x, factor.f_y);
}

ParallaxFactor mgc::parts::BasicSelectbox::get_parallax_factor() const {
    return selectbox_get_parallax_factor(&selectbox_);
}

// [feature] ItemSelectable
void mgc::parts::BasicSelectbox::set_selected_index(int32_t index) {
    selectbox_set_selected_idx(&selectbox_, static_cast<size_t>(index));
}

int32_t mgc::parts::BasicSelectbox::get_selected_index() const {
    return selectbox_get_selected_idx(&selectbox_);
}

void mgc::parts::BasicSelectbox::select_next() {
    selectbox_change_selected_idx(&selectbox_, true);
}

void mgc::parts::BasicSelectbox::select_previous() {
    selectbox_change_selected_idx(&selectbox_, false);
}

//// ItemSelectable-related
void mgc::parts::BasicSelectbox::set_cursor_symbol(const char *symbol) {
    selectbox_set_cursor_text(&selectbox_, symbol);
}

const char * mgc::parts::BasicSelectbox::get_cursor_symbol() const {
    return selectbox_get_cursor_text(&selectbox_);
}

void mgc::parts::BasicSelectbox::set_cursor_offset(int16_t x, int16_t y) {
    (void)y;
    uint8_t left_margin;
    if ( x < 0 ) {
        left_margin = 0;
    } else if ( UINT8_MAX < x ) {
        left_margin = UINT8_MAX;
    } else {
        left_margin = x;
    }
    selectbox_set_cursor_margin(&selectbox_, left_margin);
}

void mgc::parts::BasicSelectbox::set_cursor_offset(Point offset) {
    set_cursor_offset(offset.x, offset.y);
}

Point mgc::parts::BasicSelectbox::get_cursor_offset() const {
    uint8_t left_margin;
    left_margin = selectbox_.left_cursor_margin;
    return Point(left_margin, 0);
}

// [feature] Visible
bool mgc::parts::BasicSelectbox::is_visible() const {
    return selectbox_get_visible(&selectbox_);
}

void mgc::parts::BasicSelectbox::set_visible(bool v) {
    selectbox_set_visible(&selectbox_, v);
}

// [feature] Drawable
bool mgc::parts::BasicSelectbox::draw(Framebuffer &fb, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return selectbox_draw_raw(&selectbox_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
bool mgc::parts::BasicSelectbox::cell_draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return selectbox_draw_cell_raw(&selectbox_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithSize
Size mgc::parts::BasicSelectbox::get_size_impl() const {
    return selectbox_get_size(&selectbox_);
}

void mgc::parts::BasicSelectbox::set_size_impl(uint16_t width, uint16_t height) {
    (void)height; // auto-adjust
    selectbox_set_width(&selectbox_, width);
}

//// WithSize-related 
void mgc::parts::BasicSelectbox::adjust_size_to_fit() {
    selectbox_resize_to_fit(&selectbox_);
}

// [impl] WithPadding
void mgc::parts::BasicSelectbox::set_padding_impl(Padding padding) {
    selectbox_set_padding(
            &selectbox_,
            padding.top,
            padding.bottom,
            padding.left,
            padding.right
    );
}

Padding mgc::parts::BasicSelectbox::get_padding_impl() const {
    return selectbox_get_padding(&selectbox_);
}

// [impl] WithFont
void mgc::parts::BasicSelectbox::set_font_impl(const Font& font) {
    selectbox_set_font(&selectbox_, &font);
}

const Font* mgc::parts::BasicSelectbox::get_font_impl() const {
    return selectbox_get_font(&selectbox_);
}

void mgc::parts::BasicSelectbox::set_fore_color_impl(Color color) {
    selectbox_set_fore_color(&selectbox_, color);
}

Color mgc::parts::BasicSelectbox::get_fore_color_impl() const {
    return selectbox_get_fore_color(&selectbox_);
}

//// WithFont-related
void mgc::parts::BasicSelectbox::set_fontsize2x(bool fontsize2x) {
    selectbox_set_fontsize2x(&selectbox_, fontsize2x);
}

// [impl] WithItems
void mgc::parts::BasicSelectbox::add_item_impl(const char* item) {
    selectbox_append_item(&selectbox_, item);
}

void mgc::parts::BasicSelectbox::remove_item_by_index_impl(int32_t index) {
    //TODO
}

void mgc::parts::BasicSelectbox::clear_items_impl() {
    selectbox_clear_items(&selectbox_);
}

size_t mgc::parts::BasicSelectbox::get_item_count_impl() const {
    return selectbox_get_item_count(&selectbox_);
}

}// namespace parts
}// namespace mgc
