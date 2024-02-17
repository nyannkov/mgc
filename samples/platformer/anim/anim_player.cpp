/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "anim_player.hpp"
#include "resources/generates/tileset/tileset_player.h"

namespace mgc {

enum anim_idx {
    idx_invisible = 0,
    idx_walk0_l = 1,
    idx_walk1_l = 2,
    idx_walk0_r = 3,
    idx_walk1_r = 4,
    idx_crouch_l = 5,
    idx_rising_l = 6,
    idx_falling_l = 7,
    idx_crouch_r = 8,
    idx_rising_r = 9,
    idx_falling_r = 10,
};

AnimPlayer::AnimPlayer() {
    anim_type_ = AnimType::Invisible;
    tile_idx_ = idx_invisible;
}

const mgc_tileset_t *AnimPlayer::get_ptr_tileset() const {
    return &tileset_player;
}

size_t AnimPlayer::get_tile_idx() const {
    if ( tile_idx_ >= tileset_player.tile_count ) {
        MGC_WARN("Out of tile_idx");
        return idx_invisible;
    }
    return tile_idx_;
}

void AnimPlayer::set_anim_type(AnimType type) {
    anim_type_ = type;
}

void AnimPlayer::set_anim_dir(AnimDir dir) {
    anim_dir_ = dir;
}

void AnimPlayer::update() {
    switch (anim_type_) {
    case AnimType::Invisible:
        tile_idx_ = idx_invisible;
        break;
    case AnimType::Stop:
        tile_idx_ = (anim_dir_ == AnimDir::Left) ? idx_walk0_l : idx_walk0_r;
        break;
    case AnimType::Walk:
        if ( anim_dir_ == AnimDir::Left ) {
            if ( tile_idx_ == idx_walk0_l ) {
                tile_idx_ = idx_walk1_l;
            } else {
                tile_idx_ = idx_walk0_l;
            }
        } else {
            if ( tile_idx_ == idx_walk0_r ) {
                tile_idx_ = idx_walk1_r;
            } else {
                tile_idx_ = idx_walk0_r;
            }
        }
        break;
    case AnimType::Crouch:
        tile_idx_ = (anim_dir_ == AnimDir::Left) ? idx_crouch_l : idx_crouch_r;
        break;
    case AnimType::Rising:
        tile_idx_ = (anim_dir_ == AnimDir::Left) ? idx_rising_l : idx_rising_r;
        break;
    case AnimType::Falling:
        tile_idx_ = (anim_dir_ == AnimDir::Left) ? idx_falling_l : idx_falling_r;
        break;
    default:
        MGC_WARN("Invalid AnimType");
        tile_idx_ = idx_invisible;
        break;
    }
}

}
