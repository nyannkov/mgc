/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "basic_tilegrid.hpp"

namespace mgc {
namespace parts {

using mgc::math::Vec2i;
using mgc::parts::assets::TileIdMap;
using mgc::parts::assets::Tileset;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;
using mgc::graphics::DrawOptions;
using mgc::graphics::ParallaxFactor;

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

mgc_id_t mgc::parts::BasicTilegrid::id() const {
    return tilemap_get_id(&tilemap_);
}

// [features] HasPosition
Vec2i mgc::parts::BasicTilegrid::position() const {
    mgc_point_t point = tilemap_get_position(&tilemap_);
    return Vec2i(point.x, point.y);
}

void mgc::parts::BasicTilegrid::set_position(const Vec2i& position) {
    tilemap_set_position(&tilemap_, position.x, position.y);
}

// [feature] HasParallaxFactor
void mgc::parts::BasicTilegrid::set_parallax_factor(const ParallaxFactor &factor) {
    auto f = factor.to_c();
    tilemap_set_parallax_factor(&tilemap_, f.f_x, f.f_y);
}

ParallaxFactor mgc::parts::BasicTilegrid::parallax_factor() const {
    return ParallaxFactor(tilemap_get_parallax_factor(&tilemap_));
}

// [feature] Visible
bool mgc::parts::BasicTilegrid::is_visible() const {
    return tilemap_get_visible(&tilemap_);
}

void mgc::parts::BasicTilegrid::set_visible(bool v) {
    tilemap_set_visible(&tilemap_, v);
}

// [feature] Drawable
bool mgc::parts::BasicTilegrid::draw(Framebuffer &fb, const Vec2i &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = mgc_point_t{cam_pos.x, cam_pos.y};
    return tilemap_draw_raw(&tilemap_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
bool mgc::parts::BasicTilegrid::cell_draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Vec2i &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = mgc_point_t{cam_pos.x, cam_pos.y};
    return tilemap_draw_cell_raw(&tilemap_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithTileset
void mgc::parts::BasicTilegrid::set_tileset_impl(const Tileset& tileset) {
    tilemap_set_tileset(&tilemap_, &tileset);
}

const Tileset* mgc::parts::BasicTilegrid::tileset_impl() const {
    return tilemap_get_tileset(&tilemap_);
}

// [impl] WithTileIdMap
void mgc::parts::BasicTilegrid::set_tile_id_map_impl(const TileIdMap& tile_id_map) {
    tilemap_set_map(&tilemap_, &tile_id_map);
}

const TileIdMap* mgc::parts::BasicTilegrid::tile_id_map_impl() const {
    return tilemap_get_map(&tilemap_);
}

}// namespace parts
}// namespace mgc
