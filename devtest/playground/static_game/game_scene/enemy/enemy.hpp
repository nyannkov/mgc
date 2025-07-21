#ifndef MGC_ENEMY_HPP
#define MGC_ENEMY_HPP

#include <cstdlib>
#include "mgc_cpp/math/vec2.hpp"
#include "app_typedefs.hpp"
#include "game_context.hpp"
#include "game_scene/enemy/enemy_kind/enemy_kind.hpp"
#include "resources/generated/tileset/tileset_fish.h"
#include "resources/generated/btree/btree_chase.h"
#include "game_scene/enemy/bt_listener/bt_listener_enemy.hpp"

namespace app {

struct Stage;
struct Player;

struct Enemy : mgc::entities::ActorImpl<Enemy, 3>,
               mgc::features::Updatable<app::GameContext> {

    enum class EnemyState {
        LookLeft,
        LookRight,
        Chase
    };

    enum class HitboxId : mgc_id_t {
        Body = 0,
        ViewLeft,
        ViewRight,
        Count
    };

    Enemy() : is_hit_{{false, false, false}},
              bt_frame_counter_(0),
              bt_controller_(),
              bt_listener_(*this) {

        setup(EnemyKind::SkyFish);
        bt_controller_.bind_listener(bt_listener_);
    }
    ~Enemy() = default;

    void setup(EnemyKind kind) {

        if ( kind == EnemyKind::SkyFish ) {
            setup_sky_fish(*this);
        }

        bt_frame_counter_ = 0;
        enemy_kind_ = kind;
    }

    void set_position(const mgc::math::Vec2i& position) override {
        mgc::entities::ActorImpl<Enemy, 3>::set_position(position);
        this->temp_position_ = position.template cast_to<float>();
    }

    void update(app::GameContext& ctx) override {

        auto& hitboxes = this->hitboxes();

        if ( enemy_kind_ == EnemyKind::SkyFish ) {

            if ( enemy_state_ == EnemyState::Chase ) {

                const float stiffness = 0.03f;
                const float damping = 1.0f;
                auto player_pos = ctx.player_position().template cast_to<float>();

                auto delta = player_pos - temp_position_;
                auto force = delta * stiffness - velocity_ * damping;
                velocity_ = velocity_ + force;
                temp_position_ = temp_position_ + velocity_;

                if ( this->temp_position_.x < player_pos.x ) {
                    this->sprite().set_tile_index(1);
                } else {
                    this->sprite().set_tile_index(4);
                }
                
                // TODO The set_position override unintentionally clears the temporary position data
                this->sprite().set_position(temp_position_.template cast_to<int16_t>());

            } else if ( enemy_state_ == EnemyState::LookLeft ) {
                hitboxes[1].enabled = true;
                hitboxes[2].enabled = false;
                this->sprite().set_tile_index(4);

            } else if ( enemy_state_ == EnemyState::LookRight ) {
                hitboxes[1].enabled = false;
                hitboxes[2].enabled = true;
                this->sprite().set_tile_index(1);

            } else { }

            if ( bt_controller_.has_finished() ) {
                bt_controller_.reset_state();
            }

            bt_controller_.proc_until_blocked(ctx.is_any_key_pressed());

            for ( auto& h : is_hit_ ) {
                h = false;
            }
        }
    }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) { 
        if constexpr (std::is_same_v<Other, Player>) {
            is_hit_[static_cast<mgc_id_t>(info.self_hitbox.id)] = true;
        }
    }

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { 
        if constexpr (std::is_same_v<MapT, Stage>) { }
    }

private:
    mgc::math::Vec2<float> temp_position_;
    mgc::math::Vec2<float> velocity_;
    BTreeControllerT bt_controller_;
    std::array<bool, static_cast<mgc_id_t>(HitboxId::Count)> is_hit_;
    EnemyState enemy_state_;
    EnemyKind enemy_kind_;
    size_t bt_frame_counter_;

    friend BTListenerEnemy;
    BTListenerEnemy bt_listener_;

    static void setup_sky_fish(Enemy& enemy) {
        enemy.sprite().set_tileset(tileset_fish);
        enemy.sprite().set_tile_index(4);
        auto& hitboxes = enemy.hitboxes();
        // body
        hitboxes[0].id = static_cast<mgc_id_t>(HitboxId::Body);
        hitboxes[0].offset = mgc::collision::HitboxOffset(0, 0);
        hitboxes[0].size = mgc::collision::HitboxSize(16, 16);
        hitboxes[0].enabled = true;

        // view left
        hitboxes[1].id = static_cast<mgc_id_t>(HitboxId::ViewLeft);
        hitboxes[1].offset = mgc::collision::HitboxOffset(8-16*7, 8-16*5);
        hitboxes[1].size = mgc::collision::HitboxSize(16*7, 16*10);
        hitboxes[1].enabled = true;

        // view right
        hitboxes[2].id = static_cast<mgc_id_t>(HitboxId::ViewRight);
        hitboxes[2].offset = mgc::collision::HitboxOffset(8, 8-16*5);
        hitboxes[2].size = mgc::collision::HitboxSize(16*7, 16*10);
        hitboxes[2].enabled = false;

        for ( auto& h : enemy.is_hit_ ) {
            h = false;
        }

        enemy.sprite().set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(35)));
        enemy.temp_position_ = enemy.position().template cast_to<float>();
        enemy.velocity_ = mgc::math::Vec2<float>(0.0f, 0.0f);

        enemy.bt_controller_.set_btree(btree_chase);

        enemy.enemy_state_ = EnemyState::LookRight;
    }
};

}// namespace app


#endif/*MGC_ENEMY_HPP*/

