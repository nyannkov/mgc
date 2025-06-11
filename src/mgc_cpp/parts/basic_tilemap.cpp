/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "basic_tilemap.hpp"

namespace mgc {
namespace parts {

using mgc::geometry::Point;
using mgc::parts::assets::TileCollisionMap;
using mgc::parts::assets::Tileset;
using mgc::parts::types::ParallaxFactor;
using mgc::parts::types::DrawOptions;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;

void mgc::parts::BasicTilemap::reset() {
    tilemap_init(&tilemap_, 0, nullptr, nullptr);
}

// [feature] HasId
void mgc::parts::BasicTilemap::set_id(mgc_id_t id) {
    tilemap_set_id(&tilemap_, id);
}

mgc_id_t mgc::parts::BasicTilemap::get_id() const {
    return tilemap_get_id(&tilemap_);
}

// [feature] Positionable
Point mgc::parts::BasicTilemap::get_position() const {
    return Point::from_c(tilemap_get_position(&tilemap_));
}

void mgc::parts::BasicTilemap::set_position(int16_t x, int16_t y) {
    tilemap_set_position(&tilemap_, x, y);
}

// [feature] HasParallaxFactor
void mgc::parts::BasicTilemap::set_parallax_factor(const ParallaxFactor &factor) {
    tilemap_set_parallax_factor(&tilemap_, factor.f_x, factor.f_y);
}

ParallaxFactor mgc::parts::BasicTilemap::get_parallax_factor() const {
    return tilemap_get_parallax_factor(&tilemap_);
}

// [feature] HasTileCollisionMap
void mgc::parts::BasicTilemap::set_tile_collision_map(const TileCollisionMap& map) {
    tilemap_set_map(&tilemap_, &map);
}

const TileCollisionMap * mgc::parts::BasicTilemap::get_tile_collision_map() const {
    return tilemap_get_map(&tilemap_);
}

void mgc::parts::BasicTilemap::set_collision_enabled(bool enabled) {
    tilemap_set_hit_enabled(&tilemap_, enabled);
}

bool mgc::parts::BasicTilemap::get_collision_enabled() const {
    return tilemap_get_hit_enabled(&tilemap_);
}

// [feature] Visible
bool mgc::parts::BasicTilemap::is_visible() const {
    return tilemap_get_visible(&tilemap_);
}

void mgc::parts::BasicTilemap::set_visible(bool v) {
    tilemap_set_visible(&tilemap_, v);
}

// [feature] Drawable
bool mgc::parts::BasicTilemap::draw(Framebuffer &fb, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return tilemap_draw_raw(&tilemap_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
bool mgc::parts::BasicTilemap::draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return tilemap_draw_cell_raw(&tilemap_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithTileset
void mgc::parts::BasicTilemap::set_tileset_impl(const Tileset& tileset) {
    tilemap_set_tileset(&tilemap_, &tileset);
}

const Tileset* mgc::parts::BasicTilemap::get_tileset_impl() const {
    return tilemap_get_tileset(&tilemap_);
}

}// namespace parts
}// namespace mgc
