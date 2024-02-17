/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "static_object.hpp"

namespace mgc {

void StaticObject::init(mgc_id_t id, int16_t x, int16_t y) {
    hitbox_init(&hitbox_, 0, 0,  0, 16, 16);
    sprite_init(&sprite_, id);
    sprite_set_position(&sprite_, x, y);
    sprite_set_hitbox_array(&sprite_, &hitbox_, 1);
    sprite_set_tile_idx(&sprite_, 0);
    sprite_set_enabled(&sprite_, true);
}

const mgc_sprite *StaticObject::get_ptr_sprite() const {
    return &sprite_;
}

void StaticObject::set_tileset(const mgc_tileset &tileset) {
    sprite_set_tileset(&sprite_, &tileset);
}

void StaticObject::set_tile_idx(size_t idx) {
    sprite_set_tile_idx(&sprite_, idx);
}

void StaticObject::init_hitbox(mgc_id_t id, int8_t x0_ofs, int8_t y0_ofs, uint8_t width, uint8_t height) {
    hitbox_init(&hitbox_, id, x0_ofs, y0_ofs, width, height);
}

}

