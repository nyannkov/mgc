#include "glow_gate.hpp"
#include "entity/player/player.hpp"
#include "resources/generated/tileset/tileset_static_objects.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

GlowGate::GlowGate(const GamepadT& gamepad) 
    : gamepad_(gamepad),
      is_opened_(false) {

    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_static_objects);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({16, 16});
    hitboxes[0].set_enabled(false);
}

void GlowGate::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    this->mut_sprite().set_tile_index(get_tile_idx(is_opened_));
    hit_flag_ = false;
    alpha_ = 0;
}

void GlowGate::spawn(const mgc::math::Vec2i& pos, bool open) {
    is_opened_ = open;
    spawn(pos);
}

void GlowGate::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void GlowGate::open() {
    is_opened_ = true;
    this->mut_sprite().set_tile_index(get_tile_idx(is_opened_));
}

void GlowGate::close() {
    is_opened_ = false;
    this->mut_sprite().set_tile_index(get_tile_idx(is_opened_));
}

size_t GlowGate::get_tile_idx(bool is_opened) const {
    if ( is_opened ) {
        return 7;
    } else {
        return 4;
    }
}

void GlowGate::update_animation() {
    if ( hit_flag_ ) {
        if ( alpha_ > 0 ) {
            alpha_ -= 10;
            if ( alpha_ <= 0 ) {
                alpha_ = 0;
                hit_flag_ = false;
            }
        }
    }
}

void GlowGate::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    
    const auto& item = player.equipment_info().item;

    hit_flag_ = true;
    
    if ( item.has_item_at(ItemId::Lantern) ) {
        alpha_ = 23;
    }

    if ( gamepad_.just_pressed(Key::Up) ) {
        if ( is_opened_ ) {
            trigger_scene_transition_request();
        } else {
            trigger_talkflow_request();
        }
    }
}

bool GlowGate::draw(
    mgc::graphics::Framebuffer &fb,
    const mgc::math::Vec2i &cam_pos,
    const mgc::graphics::DrawOptions *options
) const {
    mgc::graphics::DrawOptions opt = { DRAW_EFFECT_ALPHA_BLEND, static_cast<uint8_t>(alpha_) };

    return Prop::draw(fb, cam_pos, &opt);
}

} // namespace prop
} // namespace app
