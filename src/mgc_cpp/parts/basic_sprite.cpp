/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */

#include "basic_sprite.hpp"

namespace mgc {
namespace parts {

using mgc::geometry::Point;
using mgc::parts::types::ParallaxFactor;
using mgc::parts::types::DrawOptions;
using mgc::parts::assets::Tileset;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;

// [feature] Resettable
void mgc::parts::BasicSprite::reset() {
    sprite_init(&sprite_, 0);
}

// [feature] HasId
void mgc::parts::BasicSprite::set_id(mgc_id_t id) {
    sprite_set_id(&sprite_, id);
}

mgc_id_t mgc::parts::BasicSprite::get_id() const {
    return sprite_get_id(&sprite_);
}

// [feature] Positionable
Point mgc::parts::BasicSprite::get_position() const {
    mgc_point_t point = sprite_get_position(&sprite_);
    return Point::from_c(point);
}

void mgc::parts::BasicSprite::set_position(int16_t x, int16_t y) {
    sprite_set_position(&sprite_, x, y);
}

// [feature] HasParallaxFactor
void mgc::parts::BasicSprite::set_parallax_factor(const ParallaxFactor &factor) {
    sprite_set_parallax_factor(&sprite_, factor.f_x, factor.f_y);
}

ParallaxFactor mgc::parts::BasicSprite::get_parallax_factor() const {
    return sprite_get_parallax_factor(&sprite_);
}

// [feature] Visible
bool mgc::parts::BasicSprite::is_visible() const {
    return sprite_get_visible(&sprite_);
}

void mgc::parts::BasicSprite::set_visible(bool v) {
    sprite_set_visible(&sprite_, v);
}

// [feature] Drawable
bool mgc::parts::BasicSprite::draw(Framebuffer &fb, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return sprite_draw_raw(&sprite_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
bool mgc::parts::BasicSprite::cell_draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return sprite_draw_cell_raw(&sprite_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithTileset
void mgc::parts::BasicSprite::set_tileset_impl(const Tileset &tileset) {
    sprite_set_tileset(&sprite_, &tileset);
    sprite_set_tile_idx(&sprite_, 0);
}

const Tileset* mgc::parts::BasicSprite::get_tileset_impl() const {
    return sprite_get_tileset(&sprite_);
}

// [impl] WithTileIndexing
void mgc::parts::BasicSprite::set_tile_index_impl(size_t idx) {
    sprite_set_tile_idx(&sprite_, idx);
}

size_t mgc::parts::BasicSprite::get_tile_index_impl() const {
    return sprite_get_tile_index(&sprite_);
}

// [impl] WithTrimRegion
void mgc::parts::BasicSprite::set_trim_region_impl(mgc::parts::types::TrimRegion region) {
    sprite_set_trim(&sprite_, region.left, region.right, region.top, region.bottom);
}

mgc::parts::types::TrimRegion mgc::parts::BasicSprite::get_trim_region_impl() const {
    return sprite_get_trim_region(&sprite_);
}

void mgc::parts::BasicSprite::clear_trim_region_impl() {
    sprite_set_trim(&sprite_, 0, 0, 0, 0);
}

bool mgc::parts::BasicSprite::has_trim_region_impl() const {
    mgc::parts::types::TrimRegion region = 
        sprite_get_trim_region(&sprite_);
    return (
        (region.left != 0) ||
        (region.right != 0 ) ||
        (region.top != 0 ) ||
        (region.bottom != 0 )
    );
}

} // namespace parts
} //namespace mgc

