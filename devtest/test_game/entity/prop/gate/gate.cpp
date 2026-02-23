#include "gate.hpp"
#include "resources/generated/tileset/tileset_static_objects.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

Gate::Gate(const GamepadT& gamepad) 
    : gamepad_(gamepad),
      is_opened_(false),
      gate_type_(GateType::Type1) {

    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_static_objects);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({16, 16});
    hitboxes[0].set_enabled(false);
}

void Gate::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    this->mut_sprite().set_tile_index(get_tile_idx(gate_type_, is_opened_));
}

void Gate::spawn(const mgc::math::Vec2i& pos, bool open) {
    is_opened_ = open;
    spawn(pos);
}

void Gate::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void Gate::open() {
    is_opened_ = true;
    this->mut_sprite().set_tile_index(get_tile_idx(gate_type_, is_opened_));
}

void Gate::close() {
    is_opened_ = false;
    this->mut_sprite().set_tile_index(get_tile_idx(gate_type_, is_opened_));
}

void Gate::set_gate_type(GateType type) {
    gate_type_ = type;
    this->mut_sprite().set_tile_index(get_tile_idx(gate_type_, is_opened_));
}

size_t Gate::get_tile_idx(GateType gate_type, bool is_opened) const {
    if ( is_opened ) {
        return (gate_type == GateType::Type1 ) ? 3 : 2;
    } else {
        return 4;
    }
}

void Gate::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Up) ) {
        if ( is_opened_ ) {
            trigger_scene_transition_request();
        } else {
            trigger_talkflow_request();
        }
    }
}

} // namespace prop
} // namespace app
