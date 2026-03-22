#include "resources/generated/talkscript/talkscript_1.h"
#include "event_shop_1.hpp"


namespace app {

using app::event::EventState;

Event_Shop1::Event_Shop1(
    SceneContext& scx,
    SceneObjects_Shop& objs
) : sound_(scx.sound),
    cp_info_(scx.world_state.checkpoint_info),
    sw_(scx.timer) {

    this->mut_sprite().set_visible(false);
    auto& hitboxes = this->mut_hitboxes();
    hitboxes[0].set_enabled(false);
    set_event_state(EventState::NotStarted);
    unlock_control();
}

void Event_Shop1::spawn(const mgc::math::Vec2i& pos) {
    set_event_state(EventState::Playing);
    lock_control();
    sw_.reset();
    sw_.start();
}

void Event_Shop1::spawn() {
    spawn({0, 0});
}

void Event_Shop1::despawn() {
    set_event_state(EventState::NotStarted);
}
void Event_Shop1::update() {
    if ( event_state() == EventState::Playing ) {
        if ( sw_.elapsed_ms() >= 1987 ) {
            mgc_node_idx_t talk_label = MGC_TALKSCRIPT_1_SHOP_TALK_1_2;
            if ( !cp_info_.has_reached(Checkpoint::Shop_FirstVisit) ) {
                cp_info_.advance_checkpoint(Checkpoint::Shop_FirstVisit);
                talk_label = MGC_TALKSCRIPT_1_SHOP_TALK_1;
            }
            set_and_trigger_talkflow_request({
                &talkscript_1,
                talk_label,
                nullptr,
                TalkflowEffectType::Type1
            });
            unlock_control();
            set_event_state(EventState::Finished);
        }
    }
}

} // namespace app

