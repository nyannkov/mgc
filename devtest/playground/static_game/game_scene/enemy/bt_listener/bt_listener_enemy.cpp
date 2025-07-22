#include "mgc_cpp/mgc.hpp"
#include "bt_listener_enemy.hpp"
#include "game_scene/enemy/enemy.hpp"


namespace app {

using LeafResult = IBTreeListenerT::LeafResult;

LeafResult BTListenerEnemy::on_proc_leaf_dispatch(
    Enemy& enemy,
    std::string_view id,
    const DurationT& duration,
    mgc_btree_tag_t tag
) {
    if ( enemy.enemy_kind_ == EnemyKind::SkyFish ) {
        if ( id == "cond/player/visible" ) {

            bool in_view = 
                enemy.is_hit_[static_cast<mgc_id_t>(Enemy::HitboxId::ViewLeft)]
             || enemy.is_hit_[static_cast<mgc_id_t>(Enemy::HitboxId::ViewRight)];
            
            if ( in_view ) {
                return LeafResult::Success;
            } else {
                return LeafResult::Failure;
            }

        } else if ( id == "action/player/chase" ) {

            enemy.enemy_state_ = Enemy::EnemyState::Chase;
            return LeafResult::Success;

        } else if ( id == "action/look/random" ) {

            if ( ++enemy.bt_frame_counter_ >= 30 ) {
                enemy.bt_frame_counter_ = 0;
                if ( ( rand() % 2 ) == 0 ) {
                    enemy.enemy_state_ = Enemy::EnemyState::LookLeft;
                } else {
                    enemy.enemy_state_ = Enemy::EnemyState::LookRight;
                }
            } else {
                if ( enemy.enemy_state_ == Enemy::EnemyState::Chase ) {
                    // Temporary hardcorded value
                    enemy.enemy_state_ = Enemy::EnemyState::LookLeft;
                }
            }
            return LeafResult::Success;

        } else {

            return LeafResult::Failure;
        }

    } else {

        return LeafResult::Failure;
    }
    
}

}// namespace app
