#include "signboard.hpp"
#include "game_context/game_context.hpp"
#include "resources/generated/tileset/tileset_static_objects.h"

namespace app {
namespace prop {

using mgc::platform::input::Key;

Signboard::Signboard(const GamepadT& gamepad) 
    : gamepad_(gamepad) {

    this->sprite().set_visible(false);
    this->sprite().set_tileset(tileset_static_objects);
    auto& hitboxes = this->hitboxes();
    hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
    hitboxes[0].size = mgc::collision::HitboxSize(16, 16);
    hitboxes[0].enabled = false;
}

void Signboard::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = true;
    this->set_position(pos);
    this->sprite().set_tile_index(1);
    this->sprite().set_visible(true);
}

void Signboard::despawn() {
    auto& hitboxes = this->hitboxes();
    hitboxes[0].enabled = false;
    this->sprite().set_visible(false);
}

void Signboard::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Up) ) {
        trigger_talkflow_request();
    }
}

} // namespace prop
} // namespace app

