#include "skyfish_bt.hpp"
#include "resources/generated/btree/btree_chase.h"

namespace app {
namespace enemy {


SkyFishBTListener::SkyFishBTListener(const FrameTimerT& frame_timer) 
    : sw_(frame_timer),
      behavior_state_(SkyFishBehaviorState::LookRight) {

    clear_all_hit_flags();

    sw_.start();
}

SkyFishBehaviorState SkyFishBTListener::behavior_state() const {
    return behavior_state_;
}

void SkyFishBTListener::set_hit_flag(SkyFishHitboxIndex index) {
    at(hit_flags_, index) = true;
}

void SkyFishBTListener::set_hit_flag(size_t index) {
    if ( index < static_cast<size_t>(SkyFishHitboxIndex::Count) ) {
        hit_flags_[index] = true;
    }
}

void SkyFishBTListener::clear_all_hit_flags() {
    for ( auto& h : hit_flags_ ) { h = false; }
}

const mgc::parts::assets::BTree& SkyFishBTListener::btree() const {
    return btree_chase;
}

SkyFishBTListener::LeafResult SkyFishBTListener::on_proc_leaf(
        std::string_view id,
        const DurationT& duration,
        mgc_btree_tag_t tag
) {
    if ( id == "cond/player/visible" ) {
        const bool in_view = 
            at(hit_flags_, SkyFishHitboxIndex::ViewLeft) ||
            at(hit_flags_, SkyFishHitboxIndex::ViewRight);

        if ( in_view ) {
            return LeafResult::Success;
        } else {
            return LeafResult::Failure;
        }
    } else if ( id == "action/player/chase" ) {
        behavior_state_ = SkyFishBehaviorState::Chase;
        return LeafResult::Success;

    } else if ( id == "action/look/random" ) {
        if ( sw_.elapsed_ms() >= 1000 ) {
            sw_.restart();
            if ( ( rand() % 2 ) == 0 ) {
                behavior_state_ = SkyFishBehaviorState::LookRight;
            } else {
                behavior_state_ = SkyFishBehaviorState::LookLeft;
            }
        }
        return LeafResult::Success;

    } else {
        return LeafResult::Failure;
    }
}

}// namespace enemy
}// namespace app

