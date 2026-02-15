#ifndef MGC_ENEMY_SKY_FISH_BT_HPP
#define MGC_ENEMY_SKY_FISH_BT_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/enemy/enemy_hitbox_index.hpp"

namespace app {
namespace enemy {

enum class SkyFishBehaviorState {
    LookLeft,
    LookRight,
    Chase
};

struct SkyFishBTListener : IBTListenerT {

    explicit SkyFishBTListener(const FrameTimerT& frame_tmer);
    SkyFishBehaviorState behavior_state() const;
    void set_hit_flag(EnemyHitboxIndex index);
    void set_hit_flag(size_t index);
    void clear_all_hit_flags();
    const mgc::parts::assets::BTree& btree() const;
    LeafResult on_proc_leaf(
            std::string_view id,
            const DurationT& duration,
            mgc_btree_tag_t tag
    ) override; 

private:
    SkyFishBehaviorState behavior_state_;
    StopwatchT sw_;
    std::array<bool, static_cast<size_t>(EnemyHitboxIndex::Count)>  hit_flags_;

};



}// namespace enemy
}// namespace app

#endif/*MGC_ENEMY_SKY_FISH_BT_HPP*/

