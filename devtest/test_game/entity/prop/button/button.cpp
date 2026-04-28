#include "button.hpp"
#include "resources/generated/tileset/tileset_static_objects.h"
#include "resources/mml/mml.h"

namespace app {
namespace prop {

Button::Button(ButtonType type) {
    
    type_ = type;

    auto& hitboxes = this->mut_hitboxes();
    if ( type == ButtonType::Type1 ) {
        hitboxes[0].set_offset({0, 0});
        hitboxes[0].set_size({16, 16});
        hitboxes[0].set_enabled(false);
        this->mut_sprite().set_tileset(tileset_static_objects);
        this->mut_sprite().set_tile_index(5);
    } else {
        hitboxes[0].set_offset({0, 5});
        hitboxes[0].set_size({16, 11});
        hitboxes[0].set_enabled(false);
        this->mut_sprite().set_tileset(tileset_static_objects);
        this->mut_sprite().set_tile_index(8);
    }
}

void Button::spawn(const mgc::math::Vec2i& pos, bool pushed) {
    
    pushed_ = pushed;
    spawn(pos);
}

void Button::spawn(const mgc::math::Vec2i& pos) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    update_sprite();
}

void Button::set_pushed_state(bool pushed) {
    pushed_ = pushed;
    just_pushed_ = false;// 
    update_sprite();
}

void Button::toggle_state() {
    set_pushed_state(!pushed_);
}

void Button::update_sprite() {
    if ( type_ == ButtonType::Type1 ) {
        if ( pushed_ ) {
            this->mut_sprite().set_tile_index(6);
        } else {
            this->mut_sprite().set_tile_index(5);
        }
    } else {
        if ( pushed_ ) {
            this->mut_sprite().set_tile_index(9);
        } else {
            this->mut_sprite().set_tile_index(8);
        }
    }
}

void Button::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void Button::on_attack_hit(
    const attack::Attack& attack,
    const mgc::collision::BoxCollisionInfo& info
) { 
    if ( !pushed_ ) {
        pushed_ = true;
        just_pushed_ = true;
        update_sprite();
    }
}

} // namespace prop
} // namespace app

