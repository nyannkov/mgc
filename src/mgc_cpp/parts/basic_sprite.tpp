/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_PARTS_BASIC_SPRITE_TPP
#define MGC_PARTS_BASIC_SPRITE_TPP

template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::reset() {
    sprite_init(&sprite_, 0);
    hitbox_array_.reset();
    sprite_set_hitbox_array(&sprite_, hitbox_array_.raw(), MaxHitboxCount);
}

// [feature] HasId
template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::set_id(mgc_id_t id) {
    sprite_set_id(&sprite_, id);
}

template <size_t MaxHitboxCount>
mgc_id_t mgc::parts::BasicSprite<MaxHitboxCount>::get_id() const {
    return sprite_get_id(&sprite_);
}

// [feature] Positionable
template <size_t MaxHitboxCount>
mgc::geometry::Point mgc::parts::BasicSprite<MaxHitboxCount>::get_position() const {
    mgc_point_t point = sprite_get_position(&sprite_);
    return mgc::geometry::Point::from_c(point);
}

template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::set_position(int16_t x, int16_t y) {
    sprite_set_position(&sprite_, x, y);
}

// [feature] HasParallaxFactor
template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::set_parallax_factor(const mgc::parts::types::ParallaxFactor &factor) {
    sprite_set_parallax_factor(&sprite_, factor.f_x, factor.f_y);
}

template <size_t MaxHitboxCount>
mgc::parts::types::ParallaxFactor mgc::parts::BasicSprite<MaxHitboxCount>::get_parallax_factor() const {
    return sprite_get_parallax_factor(&sprite_);
}

// [feature] HasHitbox
template <size_t MaxHitboxCount>
const mgc::collision::Hitbox* mgc::parts::BasicSprite<MaxHitboxCount>::get_hitbox(size_t index) const {
    return hitbox_array_.get(index);
}

template <size_t MaxHitboxCount>
const mgc::collision::Hitbox* mgc::parts::BasicSprite<MaxHitboxCount>::get_hitbox_by_id(mgc_id_t hitbox_id) const {
    return hitbox_array_.get_by_id(hitbox_id);
}

template <size_t MaxHitboxCount>
size_t mgc::parts::BasicSprite<MaxHitboxCount>::hitbox_count() const {
    return hitbox_array_.count();
}

// [feature] Visible
template <size_t MaxHitboxCount>
bool mgc::parts::BasicSprite<MaxHitboxCount>::is_visible() const {
    return sprite_get_visible(&sprite_);
}

template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::set_visible(bool v) {
    sprite_set_visible(&sprite_, v);
}

// [feature] Drawable
template <size_t MaxHitboxCount>
bool mgc::parts::BasicSprite<MaxHitboxCount>::draw(mgc::graphics::Framebuffer &fb, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return sprite_draw_raw(&sprite_, fb.data(), fb.width(), fb.height(), &c_pos, options);
}

// [feature] CellDrawable
template <size_t MaxHitboxCount>
bool mgc::parts::BasicSprite<MaxHitboxCount>::draw(mgc::graphics::CellBuffer &cb, int16_t cell_x, int16_t cell_y, const mgc::geometry::Point &cam_pos, const mgc::parts::types::DrawOptions *options) const {
    mgc_point_t c_pos = cam_pos.to_c();
    return sprite_draw_cell_raw(&sprite_, cb.data(), cell_x, cell_y, &c_pos, options);
}

// [impl] WithTileset
template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::set_tileset_impl(const mgc::parts::assets::Tileset &tileset) {
    sprite_set_tileset(&sprite_, &tileset);
    sprite_set_tile_idx(&sprite_, 0);
}

template <size_t MaxHitboxCount>
const mgc::parts::assets::Tileset* mgc::parts::BasicSprite<MaxHitboxCount>::get_tileset_impl() const {
    return sprite_get_tileset(&sprite_);
}

// [impl] WithTileIndexing
template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::set_tile_index_impl(size_t idx) {
    sprite_set_tile_idx(&sprite_, idx);
}

template <size_t MaxHitboxCount>
size_t mgc::parts::BasicSprite<MaxHitboxCount>::get_tile_index_impl() const {
    return sprite_get_tile_index(&sprite_);
}

// [impl] WithTrimRegion
template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::set_trim_region_impl(mgc::parts::types::TrimRegion region) {
    sprite_set_trim(&sprite_, region.left, region.right, region.top, region.bottom);
}

template <size_t MaxHitboxCount>
mgc::parts::types::TrimRegion mgc::parts::BasicSprite<MaxHitboxCount>::get_trim_region_impl() const {
    return sprite_get_trim_region(&sprite_);
}

template <size_t MaxHitboxCount>
void mgc::parts::BasicSprite<MaxHitboxCount>::clear_trim_region_impl() {
    sprite_set_trim(&sprite_, 0, 0, 0, 0);
}

template <size_t MaxHitboxCount>
bool mgc::parts::BasicSprite<MaxHitboxCount>::has_trim_region_impl() const {
    mgc::parts::types::TrimRegion region = 
        sprite_get_trim_region(&sprite_);
    return (
        (region.left != 0) ||
        (region.right != 0 ) ||
        (region.top != 0 ) ||
        (region.bottom != 0 )
    );
}

#endif/*MGC_PARTS_BASIC_SPRITE_TPP*/
