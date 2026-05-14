#include "attack.hpp"
#include "resources/generated/anim/attack/anim_attack.h"
#include "resources/mml/mml.h"
#include "entity/player/player.hpp"
#include "entity/enemy/enemy.hpp"

namespace app {
namespace attack {

Attack::Attack(
    const FrameTimerT& frame_timer, 
    const GamepadT& gamepad,
    SoundControllerT& sound,
    const mgc::features::HasPosition<mgc::math::Vec2i>& owner_pos
) : anim_(frame_timer),
    gamepad_(gamepad),
    sound_(sound),
    owner_pos_(owner_pos) { 

    this->set_position({0, 0});
    this->mut_sprite().set_visible(false);


    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({8, 16});
    hitboxes[0].set_enabled(false);

    hit_ = false;
}

void Attack::spawn(
    const mgc::math::Vec2i& pos,
    AttackType type,
    AttackOwner owner,
    AttackDirection dir
) {

    set_attack_type(type);

    switch ( attack_type() ) {
    case AttackType::Scratch:
        spawn_scratch(pos, owner, dir);
        break;
    case AttackType::Boomerang:
        spawn_boomerang(pos, owner, dir);
        break;
    case AttackType::Yoyo:
        spawn_yoyo(pos, owner, dir);
        break;
    default:
        break;
    }

    pos_orig_ = this->position();
    hit_ = false;
}

void Attack::despawn() {
    switch ( attack_type() ) {
    case AttackType::Scratch:
        despawn_scratch();
        break;
    case AttackType::Boomerang:
        despawn_boomerang();
        break;
    case AttackType::Yoyo:
        despawn_yoyo();
        break;
    default:
        break;
    }
}

void Attack::update_movement() {
    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    switch ( attack_type() ) {
    case AttackType::Scratch:
        update_movement_scratch();
        break;
    case AttackType::Boomerang:
        update_movement_boomerang();
        break;
    case AttackType::Yoyo:
        update_movement_yoyo();
        break;
    default:
        break;
    }
    hit_ = false;
}

void Attack::update_animation() {

    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    switch ( attack_type() ) {
    case AttackType::Scratch:
        update_animation_scratch();
        break;
    case AttackType::Boomerang:
        update_animation_boomerang();
        break;
    case AttackType::Yoyo:
        update_animation_yoyo();
        break;
    default:
        break;
    }
}

int32_t Attack::apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const {
    int32_t amount = 0;
    if ( attack_hitbox_index == 0 ) {
        if ( owner_type() == AttackOwner::Player ) {
            amount = 1;
            enemy.receive_damage(amount);
            switch (this->direction()) {
            case AttackDirection::Right:
            case AttackDirection::UpRight:
                enemy.receive_impact({12.0f, 0.0f});
                break;
            case AttackDirection::Left:
            case AttackDirection::UpLeft:
                enemy.receive_impact({-12.0f, 0.0f});
                break;
            default:
                break;
            }
        }
    }
    return amount;
}

int32_t Attack::apply_damage_to(Player& player, size_t attack_hitbox_index) const {
    int32_t amount = 0;
    if ( attack_hitbox_index == 0 ) {
        if ( owner_type() == AttackOwner::Enemy ) {
            amount = 1;
            player.receive_damage(amount);

            switch (this->direction()) {
            case AttackDirection::Right:
            case AttackDirection::UpRight:
                player.receive_impact({12.0f, 0.0f});
                break;
            case AttackDirection::Left:
            case AttackDirection::UpLeft:
                player.receive_impact({-12.0f, 0.0f});
                break;
            default:
                break;
            }
        }
    }

    return amount;
}

void Attack::spawn_scratch(
    const mgc::math::Vec2i& pos,
    AttackOwner owner,
    AttackDirection dir
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    hitboxes[0].set_size({8, 16});
    this->set_direction(dir);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    switch (this->direction()) {
    case AttackDirection::Right:
    case AttackDirection::UpRight:
        hitboxes[0].set_offset({0, 0});
        anim_.set_anim_frames(anim_attack_scratch_right);
        anim_.set_loop(false);
        break;
    default:
        hitboxes[0].set_offset({8, 0});
        anim_.set_anim_frames(anim_attack_scratch_left);
        anim_.set_loop(false);
        break;
    }

    set_lifecycle(AttackLifeCycle::Spawned);
    anim_.set_current_frame(this->mut_sprite());
    anim_.start_animation();
    sound_.play_sound_effect(MML_SE_4_ATTACK_SCRATCH);
}

void Attack::despawn_scratch() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void Attack::update_animation_scratch() {

    if ( anim_.is_finished() ) {
        despawn();
    } else {
        anim_.proc();
        anim_.set_current_frame(this->mut_sprite());
    }
}

void Attack::update_movement_scratch() {
}


void Attack::spawn_boomerang(
    const mgc::math::Vec2i& pos,
    AttackOwner owner,
    AttackDirection dir
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    hitboxes[0].set_size({8, 16});
    this->set_direction(dir);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    switch (this->direction()) {
    case AttackDirection::Right:
    case AttackDirection::UpRight:
        hitboxes[0].set_offset({0, 0});
        anim_.set_anim_frames(anim_attack_boomerang_right);
        anim_.set_loop(true);
        set_velocity({12, 0});
        break;
    default:
        hitboxes[0].set_offset({0, 0});
        anim_.set_anim_frames(anim_attack_boomerang_left);
        anim_.set_loop(true);
        set_velocity({-12, 0});
        break;
    }

    set_lifecycle(AttackLifeCycle::Spawned);
    anim_.set_current_frame(this->mut_sprite());
    anim_.start_animation();
}

void Attack::despawn_boomerang() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void Attack::update_animation_boomerang() {

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

void Attack::update_movement_boomerang() {

    if ( anim_.is_finished() || hit_ ) {
        despawn();
        return;
    }

    if ( MGC_ABS(pos_orig_.x - this->position().x) > MGC_CELL2PIXEL(10) ) {
        despawn();
        return;
    }

    auto pos = precise_position();
    auto v = velocity();
    switch (this->direction()) {
    case AttackDirection::Right:
        if ( v.x > -12 ) {
            v.x -= 1;
        }
        break;
    default:
        if ( v.x < 12 ) {
            v.x += 1;
        }
        break;
    }

    pos.x += v.x;

    set_velocity(v);
    set_precise_position(pos);
}

void Attack::spawn_yoyo(
    const mgc::math::Vec2i& pos,
    AttackOwner owner,
    AttackDirection dir
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    hitboxes[0].set_size({4, 4});
    this->set_direction(dir);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    anim_.set_anim_frames(anim_attack_yoyo);
    anim_.set_loop(false);

    hitboxes[0].set_offset({6, 6});

    switch (this->direction()) {
    case AttackDirection::Right:
        set_velocity({12, 0});
        break;
    case AttackDirection::UpRight:
        set_velocity({8, -12});
        break;
    case AttackDirection::Left:
        set_velocity({-12, 0});
        break;
    case AttackDirection::UpLeft:
        set_velocity({-8, -12});
        break;
    default:
        break;
    }

    set_lifecycle(AttackLifeCycle::Spawned);
    anim_.set_current_frame(this->mut_sprite());
    anim_.start_animation();
    sound_.play_sound_effect(MML_SE_4_ATTACK_SCRATCH);

    yoyo_sum_i_ = mgc::math::Vec2f({0, 0});
}

void Attack::despawn_yoyo() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void Attack::update_animation_yoyo() {

    if ( anim_.is_finished() ) {
        despawn();
    } else {
        anim_.proc();
        anim_.set_current_frame(this->mut_sprite());
    }
}

void Attack::update_movement_yoyo() {
    if ( anim_.is_finished() || hit_ ) {
        despawn();
        return;
    }

    auto pos = this->precise_position();
    auto o_pos = owner_pos_.position();

    float gain = 0.9;
    float k_p = 0.05;
    float k_i = 0.01;

    auto v = this->velocity();
    pos += v;

    if ( o_pos.x < pos.x ) {
        v.x--;
    } else {
        v.x++;
    }
    if ( o_pos.y < pos.y ) {
        v.y--;
    } else {
        v.y++;
    }
    this->set_velocity(v);


    float err_x = o_pos.x - pos.x;
    float err_y = o_pos.y - pos.y;
    if ( MGC_ABS(err_x) > MGC_CELL2PIXEL(10) ||
         MGC_ABS(err_y) > MGC_CELL2PIXEL(10)
    ) {
        despawn();
        return;
    }

    mgc::math::Vec2f err = {err_x, err_y};

    auto u = k_p * err + k_i * yoyo_sum_i_;

    pos += gain * u;

    this->set_precise_position(pos);

    yoyo_sum_i_ += err;
}

void Attack::on_enemy_hit(
    const enemy::Enemy& enemy,
    const mgc::collision::BoxCollisionInfo& info
) {
    switch (info.other_hitbox_index) {
    case static_cast<size_t>(enemy::EnemyHitboxIndex::Body): //fallthrough
    case static_cast<size_t>(enemy::EnemyHitboxIndex::Core):
        hit_ = true;
        break;
    default:
        break;
    }
}

void Attack::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    hit_ = true;
}

void Attack::draw_wrap(
    FramebufferT& fb,
    mgc::math::Vec2i& cam_pos
) {
    if ( attack_type_ == AttackType::Yoyo ) {
        if ( lifecycle() == AttackLifeCycle::Despawned ) {
            return;
        }

        mgc::math::Vec2i hand_ofs = {0, 0};
        switch (this->direction()) {
        case AttackDirection::Left:
            hand_ofs = mgc::math::Vec2i({0, 6});
            break;
        case AttackDirection::UpLeft:
            hand_ofs = mgc::math::Vec2i({1, 4});
            break;
        case AttackDirection::Right:
            hand_ofs = mgc::math::Vec2i({15, 6});
            break;
        case AttackDirection::UpRight:
            hand_ofs = mgc::math::Vec2i({14, 4});
            break;
        default:
            break;
        }

        auto pos = this->position() + mgc::math::Vec2i({8, 8});
        auto o_pos = owner_pos_.position() + hand_ofs;

        int16_t dx =pos.x - o_pos.x;
        int16_t dy =pos.y - o_pos.y;

        float r = sqrt(dx*dx + dy*dy);
        if ( r >= 1 ) {
            float cos_theta = dx / r;
            float sin_theta = dy / r;

            int16_t r_max = static_cast<int16_t>(r);

            for ( int16_t i = 0; i < r_max; i++ ) {
                int16_t x = static_cast<int16_t>(i * cos_theta) + o_pos.x - cam_pos.x;
                int16_t y = static_cast<int16_t>(i * sin_theta) + o_pos.y - cam_pos.y;
                fb.draw_pixel(x, y, MGC_COLOR(0.7, 0.7, 0.7));
            }
        }
    }

    this->draw(fb, cam_pos);
}

}// namespace attack
}// namespace app

