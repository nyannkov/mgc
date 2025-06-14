/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "basic_tilegrid.hpp"

namespace mgc {
namespace parts {

using mgc::geometry::Point;
using mgc::parts::assets::TileIdMap;
using mgc::parts::assets::Tileset;
using mgc::parts::assets::TileIdMap;
using mgc::parts::types::ParallaxFactor;
using mgc::parts::types::DrawOptions;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;

// [feature] Resettable
void mgc::parts::BasicTilegrid::reset() {
    tilemap_init(&tilemap_, 0, nullptr, nullptr);
    listener_ = nullptr;
    callbacks_ = mgc_tilemap_callbacks_t {
        this,
        on_request_tile_id_wrapper
    };
    tilemap_set_callbacks(&tilemap_, &callbacks_);
}

void mgc::parts::BasicTilegrid::bind_listener(IBasicTilegridListener& listener) {
    listener_ = &listener;
}

void mgc::parts::BasicTilegrid::unbind_listener() {
    listener_ = nullptr;
}

// [feature] HasId
void mgc::parts::BasicTilegrid::set_id(mgc_id_t id) {
    tilemap_set_id(&tilemap_, id);
}

mgc_id_t mgc::parts::BasicTilegrid::get_id() const {
    return tilemap_get_id(&tilemap_);
}

// [feature] Positionable
Point mgc::parts::BasicTilegrid::get_position() const {
    return Point::from_c(tilemap_get_position(&tilemap_));
}

void mgc::parts::BasicTilegrid::set_position(int16_t x, int16_t y) {
    tilemap_set_position(&tilemap_, x, y);
}

// [feature] HasParallaxFactor
void mgc::parts::BasicTilegrid::set_parallax_factor(const ParallaxFactor &factor) {
    tilemap_set_parallax_factor(&tilemap_, factor.f_x, factor.f_y);
}

ParallaxFactor mgc::parts::BasicTilegrid::get_parallax_factor() const {
    return tilemap_get_parallax_factor(&tilemap_);
}

// [feature] Visible
bool mgc::parts::BasicTilegrid::is_visible() const {
    return tilemap_get_visible(&tilemap_);
}

void mgc::parts::BasicTilegrid::set_visible(bool v) {
    tilemap_set_visible(&tilemap_, v);
}

// [feature] Drawable
bool mgc::parts::BasicTilegrid::draw(Framebuffer &fb, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return tilemap_draw_raw(&tilemap_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
bool mgc::parts::BasicTilegrid::draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Point &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return tilemap_draw_cell_raw(&tilemap_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithTileset
void mgc::parts::BasicTilegrid::set_tileset_impl(const Tileset& tileset) {
    tilemap_set_tileset(&tilemap_, &tileset);
}

const Tileset* mgc::parts::BasicTilegrid::get_tileset_impl() const {
    return tilemap_get_tileset(&tilemap_);
}

// [impl] WithTileIdMap
void mgc::parts::BasicTilegrid::set_tile_id_map_impl(const TileIdMap& tile_id_map) {
    tilemap_set_map(&tilemap_, &tile_id_map);
}

const TileIdMap* mgc::parts::BasicTilegrid::get_tile_id_map_impl() const {
    return tilemap_get_map(&tilemap_);
}

}// namespace parts
}// namespace mgc
