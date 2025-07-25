/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */

#include "basic_sprite.hpp"

namespace mgc {
namespace parts {

using mgc::math::Vec2i;
using mgc::parts::types::TrimRegion;
using mgc::parts::assets::Tileset;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;
using mgc::graphics::DrawOptions;
using mgc::graphics::ParallaxFactor;

// [feature] Resettable
void mgc::parts::BasicSprite::reset() {
    sprite_init(&sprite_, 0);
}

// [feature] HasId
void mgc::parts::BasicSprite::set_id(mgc_id_t id) {
    sprite_set_id(&sprite_, id);
}

mgc_id_t mgc::parts::BasicSprite::id() const {
    return sprite_get_id(&sprite_);
}

// [features] HasPosition
Vec2i mgc::parts::BasicSprite::position() const {
    mgc_point_t point = sprite_get_position(&sprite_);
    return Vec2i(point.x, point.y);
}

void mgc::parts::BasicSprite::set_position(const Vec2i& position) {
    sprite_set_position(&sprite_, position.x, position.y);
}

// [feature] HasParallaxFactor
void mgc::parts::BasicSprite::set_parallax_factor(const ParallaxFactor &factor) {
    auto f = factor.to_c();
    sprite_set_parallax_factor(&sprite_, f.f_x, f.f_y);
}

ParallaxFactor mgc::parts::BasicSprite::parallax_factor() const {
    return ParallaxFactor(sprite_get_parallax_factor(&sprite_));
}

// [feature] Visible
bool mgc::parts::BasicSprite::is_visible() const {
    return sprite_get_visible(&sprite_);
}

void mgc::parts::BasicSprite::set_visible(bool v) {
    sprite_set_visible(&sprite_, v);
}

// [feature] Drawable
bool mgc::parts::BasicSprite::draw(Framebuffer &fb, const Vec2i &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = mgc_point_t{cam_pos.x, cam_pos.y};
    return sprite_draw_raw(&sprite_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
bool mgc::parts::BasicSprite::cell_draw(CellBuffer &cb, int16_t cell_x, int16_t cell_y, const Vec2i &cam_pos, const DrawOptions *options) const {
    mgc_point_t c_pos = mgc_point_t{cam_pos.x, cam_pos.y};
    return sprite_draw_cell_raw(&sprite_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithTileset
void mgc::parts::BasicSprite::set_tileset_impl(const Tileset &tileset) {
    sprite_set_tileset(&sprite_, &tileset);
    sprite_set_tile_idx(&sprite_, 0);
}

const Tileset* mgc::parts::BasicSprite::tileset_impl() const {
    return sprite_get_tileset(&sprite_);
}

// [impl] WithTileIndexing
void mgc::parts::BasicSprite::set_tile_index_impl(size_t idx) {
    sprite_set_tile_idx(&sprite_, idx);
}

size_t mgc::parts::BasicSprite::tile_index_impl() const {
    return sprite_get_tile_index(&sprite_);
}

// [impl] WithTrimRegion
void mgc::parts::BasicSprite::set_trim_region_impl(TrimRegion region) {
    auto trim = region.to_c();
    sprite_set_trim(&sprite_, trim.left, trim.right, trim.top, trim.bottom);
}

TrimRegion mgc::parts::BasicSprite::trim_region_impl() const {
    return TrimRegion(sprite_get_trim_region(&sprite_));
}

void mgc::parts::BasicSprite::clear_trim_region_impl() {
    sprite_set_trim(&sprite_, 0, 0, 0, 0);
}

bool mgc::parts::BasicSprite::has_trim_region_impl() const {
    auto trim = 
        TrimRegion(sprite_get_trim_region(&sprite_));
    return (
        (trim.left != 0) ||
        (trim.right != 0 ) ||
        (trim.top != 0 ) ||
        (trim.bottom != 0 )
    );
}

} // namespace parts
} //namespace mgc

