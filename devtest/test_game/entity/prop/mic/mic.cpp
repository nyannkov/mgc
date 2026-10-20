#include "mic.hpp"
#include "resources/generated/tileset/tileset_static_objects_20x32.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

Mic::Mic(const GamepadT& gamepad)
    : gamepad_(gamepad) {

    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({8, 8});
    hitboxes[0].set_enabled(false);
}

void Mic::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    this->mut_sprite().set_tileset(tileset_static_objects_20x32);
    this->mut_sprite().set_tile_index(0); //microphone
}

void Mic::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void Mic::update_animation() {
}

void Mic::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
}

} // namespace prop
} // namespace app

