#include "teleporter.hpp"
#include "resources/generated/tileset/tileset_static_objects.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

Teleporter::Teleporter(const GamepadT& gamepad) 
    : gamepad_(gamepad) {

    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_static_objects);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({16, 16});
    hitboxes[0].set_enabled(false);
}

void Teleporter::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    this->mut_sprite().set_tile_index(2);
}

void Teleporter::spawn(const mgc::math::Vec2i& pos, const mgc::math::Vec2i& dest) {
    set_destination(dest);
    spawn(pos);
}

void Teleporter::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void Teleporter::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Up) ) {
        entry_flag_ = true;
    }
}

} // namespace prop
} // namespace app


