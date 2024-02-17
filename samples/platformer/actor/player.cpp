/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "system/system.h"
#include "player.hpp"

namespace mgc {

void Player::init(mgc_id_t id, int16_t x, int16_t y) {
    hitbox_init(&hitbox_array_[HITBOX_ID_BODY], HITBOX_ID_BODY, 2,  2, 12, 12);
    hitbox_init(&hitbox_array_[HITBOX_ID_HEAD], HITBOX_ID_HEAD, 2,  0, 12,  1);
    hitbox_init(&hitbox_array_[HITBOX_ID_FOOT], HITBOX_ID_FOOT, 2, 15, 12,  1);
    sprite_init(&sprite_, id);
    sprite_set_position(&sprite_, x, y);
    sprite_set_hitbox_array(&sprite_, hitbox_array_, NUM_OF_HITBOX_ID);
    sprite_set_tileset(&sprite_, anim_player_.get_ptr_tileset());
    sprite_set_tile_idx(&sprite_, 0);
    sprite_set_enabled(&sprite_, true);
    jumpctrl_init(&jumpctrl_);

    mark_x_ = sprite_.x;
    mark_y_ = sprite_.y;
    vx_ = 0;
    vy_ = 0;
    foot_hit_ = false;
}

void Player::update() {
    auto gamepad = sys_get_gamepad_driver();
    int32_t gp_x = gamepad->get_x_axis();
    int32_t gp_y = gamepad->get_y_axis();
    enum gp_key_state key1_stat = gamepad->get_key_state(GP_KEY_0);

    mark_current_position();

    if ( gp_x > 700 ) vx_ = 4;
    else if ( gp_x < -700 ) vx_ = -4;
    else vx_ = 0;

    jumpctrl_update_jump_state(&jumpctrl_, key1_stat==GP_KEY_STATE_ON, foot_hit_);
    foot_hit_ = false;

    if ( jumpctrl_get_jump_state(&jumpctrl_) == MGC_JUMP_STATE_FLOATING ) {
        vy_ = -10;
    } else {
        vy_++;
        if ( vy_ > 14 ) {
            vy_ = 14;
        } else if ( vy_ < -14 ) {
            vy_ = -14;
        } else {
        }
    }
    set_position(sprite_.x + vx_, sprite_.y + vy_);
}

void Player::set_anim(AnimType type, AnimDir dir) {
    anim_player_.set_anim_type(type);
    anim_player_.set_anim_dir(dir);
    anim_player_.update();
    sprite_set_tile_idx(&sprite_, anim_player_.get_tile_idx());
}

void Player::update_anim() {
    if ( vx_ < 0 ) {
        anim_player_.set_anim_dir(AnimDir::Left);
    } else if ( vx_ > 0 ) {
        anim_player_.set_anim_dir(AnimDir::Right);
    } else {
        // No changed.
    }

    if ( foot_hit_ ) {
        if ( vx_ == 0 ) {
            anim_player_.set_anim_type(AnimType::Stop);
        } else {
            anim_player_.set_anim_type(AnimType::Walk);
        }
    } else {
        if ( vy_ > 0 ) {
            anim_player_.set_anim_type(AnimType::Falling);
        } else if ( vy_ < 0 ) {
            anim_player_.set_anim_type(AnimType::Rising);
        } else {
            // No changed.
        }
    }

    anim_player_.update();
    sprite_set_tile_idx(&sprite_, anim_player_.get_tile_idx());
}

void Player::set_position(int16_t x, int16_t y) {
    sprite_set_position(&sprite_, x, y);
}

void Player::mark_current_position() {
    mark_x_ = sprite_.x;
    mark_y_ = sprite_.y;
}

const mgc_sprite *Player::get_ptr_sprite() const {
    return &sprite_;
}

void Player::on_hit_tilemap(const mgc_hitbox &hitbox, int16_t map_x, int16_t map_y) {
    int16_t temp_x, temp_y;
    int16_t prev_x, prev_y;

    temp_x = sprite_.x + hitbox.x0_ofs;
    temp_y = sprite_.y + hitbox.y0_ofs;
    prev_x = mark_x_ + hitbox.x0_ofs;
    prev_y = mark_y_ + hitbox.y0_ofs;

    switch (hitbox.id) {
    case HITBOX_ID_BODY:
        if ( ( (prev_y + hitbox.height) > map_y ) && ( prev_y < (map_y + MGC_CELL_LEN) ) ||
             ( (map_y <= temp_y) && ((temp_y + hitbox.height) <= (map_y + MGC_CELL_LEN)) )
        ) {
            if ( vx_ > 0 ) {
                temp_x = map_x - hitbox.width;
            } else if ( vx_ < 0 ) {
                temp_x = map_x + MGC_CELL_LEN;
            } else {
            }
            vx_ = 0;
        }
        break;

    case HITBOX_ID_HEAD:
        if ( vy_ < 0 ) {
            temp_y = map_y + MGC_CELL_LEN;
            vy_ = 0;
        }
        break;

    case HITBOX_ID_FOOT:
        if ( vy_ > 0 ) {
            temp_y = map_y - hitbox.height;
            vy_ = 0;
        }
        foot_hit_ = true;
        break;

    default:
        MGC_WARN("Unexpected hitbox id");
        break;
    }

    set_position(temp_x - hitbox.x0_ofs, temp_y - hitbox.y0_ofs);
}

}

