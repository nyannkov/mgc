#include "door.hpp"
#include "resources/generated/tileset/tileset_static_objects_16x24.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

Door::Door(const GamepadT& gamepad) 
    : gamepad_(gamepad),
      is_locked_(true) {

    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_static_objects_16x24);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({1, 0});
    hitboxes[0].set_size({16, 24});
    hitboxes[0].set_enabled(false);
}

void Door::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    this->mut_sprite().set_tile_index(0);
}

void Door::spawn(const mgc::math::Vec2i& pos, bool locked) {
    is_locked_ = locked;
    spawn(pos);
}

void Door::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void Door::lock() {
    is_locked_ = true;
}

void Door::unlock() {
    is_locked_ = false;
}

void Door::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Up) ) {
        trigger_talkflow_request();
    }
}

} // namespace prop
} // namespace app
