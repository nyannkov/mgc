#include "florist.hpp"
#include "resources/generated/tileset/tileset_florist.h"

namespace app {
namespace civilian {

using mgc::platform::input::Key;

Florist::Florist(const GamepadT& gamepad, CheckpointInfo& cp_info, Player& player) 
    : gamepad_(gamepad),
      cp_info_(cp_info),
      talkflow_listener_(player) {
    this->mut_sprite().set_visible(false);
    this->mut_sprite().set_tileset(tileset_florist);
    auto& hitboxes = this->mut_hitboxes();

    at(hitboxes, CivilHitboxIndex::Body).set_offset({0, 0});
    at(hitboxes, CivilHitboxIndex::Body).set_size({16, 24});
    at(hitboxes, CivilHitboxIndex::Body).set_enabled(false);
    at(hitboxes, CivilHitboxIndex::View).set_offset({-16, 0});
    at(hitboxes, CivilHitboxIndex::View).set_size({38, 24});
    at(hitboxes, CivilHitboxIndex::View).set_enabled(false);
}

void Florist::spawn(const mgc::math::Vec2i& pos) {
    this->set_position(pos);
    auto& hitboxes = this->mut_hitboxes();
    at(hitboxes, CivilHitboxIndex::Body).set_enabled(true);
    at(hitboxes, CivilHitboxIndex::View).set_enabled(true);
    this->mut_sprite().set_visible(true);

}

void Florist::spawn(const mgc::math::Vec2i& pos, FloristAnimState anim_state) {
    this->set_anim_state(anim_state);
    this->spawn(pos);
}

void Florist::despawn() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
}

void Florist::update_movement() {
}

void Florist::update_animation() {
}

void Florist::set_anim_state(FloristAnimState anim_state) {
    switch (anim_state) {
    case FloristAnimState::Stand_Left:
        this->mut_sprite().set_tile_index(0);
        break;
    default:
        break;
    }
    anim_state_ = anim_state;
}

void Florist::on_player_in_view(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    if ( gamepad_.just_pressed(Key::Enter) ) {
        mgc_node_idx_t talk_label = MGC_TALKSCRIPT_1_SHOP_TALK_3;
        if ( !cp_info_.has_reached(Checkpoint::Shop_FirstTalk) ) {
            cp_info_.advance_checkpoint(Checkpoint::Shop_FirstTalk);
            talk_label = MGC_TALKSCRIPT_1_SHOP_TALK_2;
        }
        this->set_talkflow_request({
            &talkscript_1,
            talk_label,
            &talkflow_listener_,
            TalkflowEffectType::Type1
        });
        trigger_talkflow_request();
    }
}


}// namespace enemy
}// namespace app

