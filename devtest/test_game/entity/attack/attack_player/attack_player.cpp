#include <math.h>//sqrt
#include "attack_player.hpp"
#include "resources/generated/anim/attack/anim_attack.h"
#include "resources/generated/anim/attack_bursting/anim_attack_bursting.h"
#include "resources/mml/mml.h"
#include "entity/player/player.hpp"
#include "entity/enemy/enemy.hpp"
#include "entity/carrier/carrier.hpp"

namespace app {
namespace attack {

AttackPlayer::AttackPlayer(
    const FrameTimerT& frame_timer, 
    const GamepadT& gamepad,
    SoundControllerT& sound,
    const mgc::features::HasPosition<mgc::math::Vec2i>& owner_pos
) : anim_(frame_timer),
    gamepad_(gamepad),
    sound_(sound),
    sw_(frame_timer),
    owner_pos_(owner_pos) { 

    this->set_position({0, 0});
    this->mut_sprite().set_visible(false);


    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_offset({0, 0});
    hitboxes[0].set_size({8, 16});
    hitboxes[0].set_enabled(false);

    hit_ = false;
}

void AttackPlayer::spawn(
    const mgc::math::Vec2i& pos,
    AttackOwner owner
) {
    spawn(pos, AttackPlayerType::Scratch, owner, AttackDirection::Right, {0, 0});
}

void AttackPlayer::spawn(
    const mgc::math::Vec2i& pos,
    AttackPlayerType type,
    AttackOwner owner,
    AttackDirection dir,
    const mgc::math::Vec2f& velocity
) {

    set_attack_type(type);
    firework_phase_ = FireworkPhase::Idle;
    hold_state_ = HoldState::Off;

    set_velocity(velocity);

    switch ( attack_type() ) {
    case AttackPlayerType::Scratch:
        spawn_scratch(pos, owner, dir);
        break;
    case AttackPlayerType::Boomerang:
        spawn_boomerang(pos, owner, dir);
        break;
    case AttackPlayerType::Yoyo:
        spawn_yoyo(pos, owner, dir);
        break;
    case AttackPlayerType::Firework:
        spawn_firework(pos, owner, dir);
        break;
    case AttackPlayerType::Firework_Bursting:
        spawn_firework_bursting(pos, owner, dir);
        break;
    default:
        break;
    }

    pos_orig_ = this->position();
    hit_ = false;
}

void AttackPlayer::despawn() {
    switch ( attack_type() ) {
    case AttackPlayerType::Scratch:
        despawn_scratch();
        break;
    case AttackPlayerType::Boomerang:
        despawn_boomerang();
        break;
    case AttackPlayerType::Yoyo:
        despawn_yoyo();
        break;
    case AttackPlayerType::Firework:
        despawn_firework();
        break;
    case AttackPlayerType::Firework_Bursting:
        despawn_firework_bursting();
        break;
    default:
        break;
    }
}

void AttackPlayer::update_movement() {
    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    switch ( attack_type() ) {
    case AttackPlayerType::Scratch:
        update_movement_scratch();
        break;
    case AttackPlayerType::Boomerang:
        update_movement_boomerang();
        break;
    case AttackPlayerType::Yoyo:
        update_movement_yoyo();
        break;
    case AttackPlayerType::Firework:
        update_movement_firework();
        break;
    case AttackPlayerType::Firework_Bursting:
        update_movement_firework_bursting();
        break;
    default:
        break;
    }
    hit_ = false;
}

void AttackPlayer::update_animation() {

    if ( lifecycle() == AttackLifeCycle::Despawned ) {
        return;
    }

    switch ( attack_type() ) {
    case AttackPlayerType::Scratch:
        update_animation_scratch();
        break;
    case AttackPlayerType::Boomerang:
        update_animation_boomerang();
        break;
    case AttackPlayerType::Yoyo:
        update_animation_yoyo();
        break;
    case AttackPlayerType::Firework:
        update_animation_firework();
        break;
    case AttackPlayerType::Firework_Bursting:
        update_animation_firework_bursting();
        break;
    default:
        break;
    }
}

int32_t AttackPlayer::apply_damage_to(enemy::Enemy& enemy, size_t attack_hitbox_index) const {
    int32_t amount = 0;
    float dumping = 0.5;
    if ( attack_hitbox_index != 0 ) {
        return 0;
    }
    
    if ( owner_type() == AttackOwner::Player ) {
        if ( attack_type() == AttackPlayerType::Firework ) {
            amount = 0;
        } else if ( attack_type() == AttackPlayerType::Firework_Bursting ) {
            if ( firework_phase_ == FireworkPhase::Fading ) {
                amount = 0;
            } else {
                amount = 1;
                dumping = 0.9;
            }
        } else {
            amount = 1;
        }

        if ( amount != 0 ) {
            enemy.receive_damage(amount);
            if ( attack_type() != AttackPlayerType::Firework_Bursting ) {
                sound_.play_sound_effect(MML_SE_3_DAMAGE);
            }

            if ( attack_type() == AttackPlayerType::Firework_Bursting ) {
                
                auto pos_attack_center = this->precise_position() + mgc::math::Vec2f(32, 32);
                auto pos_enemy = enemy.precise_position();
                auto impact = pos_enemy - pos_attack_center;
                if ( impact.x != 0.0f ) {
                    impact.x = 1.0f / impact.x;
                } else {
                    impact.x = 12.0f;
                }
                if ( impact.y != 0.0f ) {
                    impact.y = 1.0f / impact.y;
                } else {
                    impact.y = 12.0f;
                }
                impact *= 1000.0f;

                if ( impact.x > 12.0f ) {
                    impact.x = 12.0f;
                } else if ( impact.x < -12.0f ) {
                    impact.x = -12.0f;
                } else { }

                if ( impact.y > 12.0f ) {
                    impact.y = 12.0f;
                } else if ( impact.y < -12.0f ) {
                    impact.y = -12.0f;
                } else { }

                enemy.receive_impact(impact, dumping);
                
            } else {
                switch (this->direction()) {
                case AttackDirection::Right:
                case AttackDirection::UpRight:
                    enemy.receive_impact({12.0f, 0.0f}, dumping);
                    break;
                case AttackDirection::Left:
                case AttackDirection::UpLeft:
                    enemy.receive_impact({-12.0f, 0.0f}, dumping);
                    break;
                default:
                    break;
                }
            }
        }
    }

    return amount;
}

int32_t AttackPlayer::apply_damage_to(Player& player, size_t attack_hitbox_index) const {
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
        } else if ( owner_type() == AttackOwner::Player ) {

            if ( attack_type() == AttackPlayerType::Firework_Bursting ) {
                if ( firework_phase_ != FireworkPhase::Fading ) {
                    amount = 1;
                    player.receive_damage(amount);
                    auto pos_attack_center = this->precise_position() + mgc::math::Vec2f(32, 32);
                    auto pos_player = player.precise_position();
                    auto impact = pos_player - pos_attack_center;
                    if ( impact.x != 0.0f ) {
                        impact.x = 1.0f / impact.x;
                    } else {
                        impact.x = 12.0f;
                    }
                    if ( impact.y != 0.0f ) {
                        impact.y = 1.0f / impact.y;
                    } else {
                        impact.y = 12.0f;
                    }

                    impact *= 1000.0f;

                    if ( impact.x > 12.0f ) {
                        impact.x = 12.0f;
                    } else if ( impact.x < -12.0f ) {
                        impact.x = -12.0f;
                    } else { }

                    if ( impact.y > 12.0f ) {
                        impact.y = 12.0f;
                    } else if ( impact.y < -12.0f ) {
                        impact.y = -12.0f;
                    } else { }

                    player.receive_impact(impact, 1.0f);
                }
            }
        } else { }
    }

    return amount;
}

void AttackPlayer::spawn_scratch(
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

void AttackPlayer::despawn_scratch() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void AttackPlayer::update_animation_scratch() {

    if ( anim_.is_finished() ) {
        despawn();
    } else {
        anim_.proc();
        anim_.set_current_frame(this->mut_sprite());
    }
}

void AttackPlayer::update_movement_scratch() {
}


void AttackPlayer::spawn_boomerang(
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

void AttackPlayer::despawn_boomerang() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void AttackPlayer::update_animation_boomerang() {

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

void AttackPlayer::update_movement_boomerang() {

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

void AttackPlayer::spawn_yoyo(
    const mgc::math::Vec2i& pos,
    AttackOwner owner,
    AttackDirection dir
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    hitboxes[0].set_size({4, 4});
    hitboxes[0].set_offset({6, 6});
    this->set_direction(dir);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);
    anim_.set_anim_frames(anim_attack_yoyo);
    anim_.set_loop(false);


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

void AttackPlayer::despawn_yoyo() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void AttackPlayer::update_animation_yoyo() {

    if ( anim_.is_finished() ) {
        despawn();
    } else {
        anim_.proc();
        anim_.set_current_frame(this->mut_sprite());
    }
}

void AttackPlayer::update_movement_yoyo() {
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

void AttackPlayer::spawn_firework(
    const mgc::math::Vec2i& pos,
    AttackOwner owner,
    AttackDirection dir
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    hitboxes[0].set_size({7, 7});
    hitboxes[0].set_offset({4, 4});
    this->set_direction(dir);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    hold_state_ = HoldState::On;
    firework_phase_ = FireworkPhase::Burning;
    anim_.set_anim_frames(anim_attack_firework_ball_1);
    anim_.set_loop(true);

    set_lifecycle(AttackLifeCycle::Spawned);
    anim_.set_current_frame(this->mut_sprite());
    anim_.start_animation();
    sw_.restart();
}

void AttackPlayer::despawn_firework() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void AttackPlayer::update_animation_firework() {
    
    if ( firework_phase_ == FireworkPhase::Burning ) {
        if ( sw_.elapsed_ms() >= 1000 ) {
            sw_.restart();
            firework_phase_ = FireworkPhase::Detonating;
            anim_.set_anim_frames(anim_attack_firework_ball_2);
            anim_.start_animation();
        }
    } else if ( firework_phase_ == FireworkPhase::Detonating ) {
        if ( sw_.elapsed_ms() >= 1000 ) {
            sw_.restart();
            firework_phase_ = FireworkPhase::Bursting;
            despawn_firework();

            //TODO
            set_attack_type(AttackPlayerType::Firework_Bursting);
            spawn_firework_bursting(
                this->position() + mgc::math::Vec2i(-32+8, -32+8),
                AttackOwner::Player,
                attack::AttackDirection::Right
            );
            sound_.play_sound_effect(MML_SE_8_OPEN);
            return;
        }
        
    } else { }

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

void AttackPlayer::update_movement_firework() {
    
    if ( hold_state_ == HoldState::On ) {
        //auto pos = precise_position();
        auto o_pos = owner_pos_.position();
        set_position(o_pos);
    } else {
        auto pos = precise_position();
        auto v = velocity();

        if ( v.y < 8.0f ) {
            v.y += 1;
        }
        pos.y += v.y;
        pos.x += v.x;

        set_velocity(v);
        set_precise_position(pos);
       
    }
}

void AttackPlayer::spawn_firework_bursting(
    const mgc::math::Vec2i& pos,
    AttackOwner owner,
    AttackDirection dir
) {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(true);
    hitboxes[0].set_size({64, 64});
    hitboxes[0].set_offset({0, 0});
    this->set_direction(dir);
    this->set_position(pos);
    this->mut_sprite().set_visible(true);

    hold_state_ = HoldState::Off;
    firework_phase_ = FireworkPhase::Bursting;
    anim_.set_anim_frames(anim_attack_bursting_firework_bursting);
    anim_.set_loop(true);

    color_mask_ = 0;
    MGC_ASSERT(
        countof(color_palette_) >= tileset_get_palette_count(this->sprite().tileset()),
        ""
    );
    tileset_copy_palette(
        this->sprite().tileset(),
        color_palette_,
        countof(color_palette_)
    );

    draw_opt_.eff_flags = static_cast<uint16_t>(DRAW_EFFECT_PALETTE_CHANGE | DRAW_EFFECT_ALPHA_BLEND);
    draw_opt_.alpha = 0;
    draw_opt_.palette_array = color_palette_;
    draw_opt_.palette_count = countof(color_palette_);

    set_lifecycle(AttackLifeCycle::Spawned);
    anim_.set_current_frame(this->mut_sprite());
    anim_.start_animation();
    sw_.restart();
}

void AttackPlayer::despawn_firework_bursting() {
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    this->mut_sprite().set_visible(false);
    set_lifecycle(AttackLifeCycle::Despawned);
}

void AttackPlayer::update_animation_firework_bursting() {

    anim_.proc();
    anim_.set_current_frame(this->mut_sprite());
}

void AttackPlayer::update_movement_firework_bursting() {
    
    if ( firework_phase_ == FireworkPhase::Fading ) {
        if ( draw_opt_.alpha == 0 ) {
            despawn_firework_bursting();
        }
    }
}

void AttackPlayer::launch(const mgc::math::Vec2f& velocity) {
    hold_state_ = HoldState::Off;
    set_velocity(velocity);
}

void AttackPlayer::on_enemy_hit(
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

void AttackPlayer::on_player_hit(
    const Player& player,
    const mgc::collision::BoxCollisionInfo& info
) {
    hit_ = true;
}

void AttackPlayer::on_collision_resolved(
    const stage::LayerBlock& block,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( attack_type_ == AttackPlayerType::Firework ) {
        // TODO phase

        auto v = velocity();
        auto pos = this->precise_position();
        if ( info.pushback.x != 0 ) {
            v.x *= -1;
        }
        if ( info.pushback.y != 0 ) {
            v.y *= -1;
        }
        v *= 0.5;
        if ( MGC_ABS(v.y) < 1 ) {
            v.y = 0;
        }

        pos.x += info.pushback.x;
        pos.y += info.pushback.y;

        //TODO
        if ( info.is_fully_blocked ) {
            pos.y += -8;
        }

        this->set_precise_position(pos);

        set_velocity(v);
    }
}

void AttackPlayer::on_collision_resolved(
    const carrier::Carrier& carrier,
    const mgc::collision::MapPushbackInfo& info
) {
    if ( attack_type_ == AttackPlayerType::Firework ) {
        // TODO phase

        auto v = velocity();
        auto pos = this->precise_position();
        if ( info.pushback.x != 0 ) {
            v.x *= -1;
        }
        if ( info.pushback.y != 0 ) {
            v.y *= -1;
        }
        v *= 0.5;
        if ( MGC_ABS(v.y) < 1 ) {
            v.y = 0;
        }

        if ( info.pushback.y < 0 ) {
            if ( carrier.delta().y > 0 ) {
                v.y = 0;
                pos.y += carrier.delta().y;
            }
        }
        pos.x += carrier.delta().x;
        pos.x += info.pushback.x;
        pos.y += info.pushback.y;

        //TODO
        if ( info.is_fully_blocked ) {
            pos.y += -8;
        }

        //this->set_position(pos);
        this->set_precise_position(pos);


        set_velocity(v);
    }
}

void AttackPlayer::draw_wrap(
    FramebufferT& fb,
    mgc::math::Vec2i& cam_pos
) {
    if ( attack_type_ == AttackPlayerType::Yoyo ) {
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

        this->draw(fb, cam_pos);

    } else if ( attack_type_ == AttackPlayerType::Firework_Bursting ) {

        tileset_copy_palette(
            this->sprite().tileset(),
            color_palette_,
            countof(color_palette_)
        );
        color_mask_+=3;
        if ( color_mask_ > 0xFF ) {
            color_mask_ = 0;
        }

        for ( int i = 0; i < 32; i++ ) {
            color_palette_[i] ^= color_mask_;
        }
        draw_opt_.palette_array = color_palette_;
        draw_opt_.palette_count = countof(color_palette_);

        if ( firework_phase_ == FireworkPhase::Fading ) {
            int16_t alpha = draw_opt_.alpha;
            alpha -= 5;
            if ( alpha < 0 ) {
                alpha = 0;
            }
            draw_opt_.alpha = static_cast<uint8_t>(alpha);
        }

        this->draw(fb, cam_pos, &draw_opt_);
        if ( firework_phase_ == FireworkPhase::Bursting ) {
            fill_color_with_alpha(fb, MGC_COLOR_WHITE, 100);
            firework_phase_ = FireworkPhase::Bursting_2;
            draw_opt_.alpha = 255;
        } else if ( firework_phase_ == FireworkPhase::Bursting_2 ) {
            firework_phase_ = FireworkPhase::Fading;
        } else { }

    } else { 

        this->draw(fb, cam_pos);
    }

}


}// namespace attack
}// namespace app

