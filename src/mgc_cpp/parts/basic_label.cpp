/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "basic_label.hpp"

namespace mgc {
namespace parts {

using mgc::geometry::Point;
using mgc::parts::types::Size;
using mgc::parts::types::ParallaxFactor;
using mgc::parts::types::DrawOptions;
using mgc::parts::assets::Font;
using mgc::graphics::Color;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;

// [features] Resettable
void mgc::parts::BasicLabel::reset() {
    label_init(&label_, 0, nullptr, false);
}

// [features] HasId
void mgc::parts::BasicLabel::set_id(mgc_id_t id) {
    label_set_id(&label_, id);
}

mgc_id_t mgc::parts::BasicLabel::get_id() const {
    return label_get_id(&label_);
}

// [features] Positionable
Point mgc::parts::BasicLabel::get_position() const {
    return Point::from_c(label_get_position(&label_));
}

void mgc::parts::BasicLabel::set_position(int16_t x, int16_t y) {
    label_set_position(&label_, x, y);
}

// [features] HasParallaxFactor
void mgc::parts::BasicLabel::set_parallax_factor(const ParallaxFactor &factor) {
    label_set_parallax_factor(&label_, factor.f_x, factor.f_y);
}

ParallaxFactor mgc::parts::BasicLabel::get_parallax_factor() const {
    return label_get_parallax_factor(&label_);
}

// [features] Visible
bool mgc::parts::BasicLabel::is_visible() const {
    return label_get_visible(&label_);
}

void mgc::parts::BasicLabel::set_visible(bool v) {
    label_set_visible(&label_, v);
}

// [features] Drawable
bool mgc::parts::BasicLabel::draw(Framebuffer &fb, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return label_draw_raw(&label_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [features] CellDrawable
bool mgc::parts::BasicLabel::cell_draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return label_draw_cell_raw(&label_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithSize
Size mgc::parts::BasicLabel::get_size_impl() const {
    return label_get_size(&label_);
}

void mgc::parts::BasicLabel::set_size_impl(uint16_t width, uint16_t height) {
    label_set_size(&label_, width, height);
}

//// WithSize-related
void mgc::parts::BasicLabel::adjust_size_to_fit() {
    label_resize_to_fit(&label_);
}


// [impl] WithFont
void mgc::parts::BasicLabel::set_font_impl(const Font& font) {
    label_set_font(&label_, &font);
}

const Font* mgc::parts::BasicLabel::get_font_impl() const {
    return label_get_font(&label_);
}

void mgc::parts::BasicLabel::set_fore_color_impl(Color color) {
    label_set_fore_color(&label_, color);
}

Color mgc::parts::BasicLabel::get_fore_color_impl() const {
    return label_get_fore_color(&label_);
}

//// WithFont-related
void mgc::parts::BasicLabel::set_fontsize2x(bool fontsize2x) {
    label_set_fontsize2x(&label_, fontsize2x);
}

Color mgc::parts::BasicLabel::get_back_color() const {
    return label_get_back_color(&label_);
}

void mgc::parts::BasicLabel::set_enabled_back_color(bool enabled) {
    label_set_enable_back_color(&label_, enabled);
}

bool mgc::parts::BasicLabel::is_back_enabled() const {
    return label_get_enable_back_color(&label_);
}

// [impl] WithText
void mgc::parts::BasicLabel::set_text_impl(const char *text) {
    label_set_text(&label_, text);
}

const char* mgc::parts::BasicLabel::get_text_impl() const {
    return label_get_text(&label_);
}

void mgc::parts::BasicLabel::clear_text_impl() {
    label_set_text(&label_, "");
}

}// namespace parts
}// namespace mgc

