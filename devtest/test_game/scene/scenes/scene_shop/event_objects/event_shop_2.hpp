#ifndef MGC_EVENT_SHOP_2_HPP
#define MGC_EVENT_SHOP_2_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "entity/event/event.hpp"
#include "entity/civilian/florist/florist.hpp"
#include "scene_objects/scene_objects_shop.hpp"
#include "utils/screen_fader/screen_fader.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

enum class EventDetail {
    Stop,
    FadeOut,
    FadeIn,
    DrinkCoffee,
    WaitFadeOut2,
    FadeOut2,
    FadeIn2
};

struct Event_Shop2 : event::Event {

    Event_Shop2(
        SceneContext& scx,
        SceneObjects_Shop& objs
    );

    void spawn(const mgc::math::Vec2i& pos) override;
    void spawn();
    void despawn() override;
    void update() override;
    void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) override;

private:
    SoundControllerT& sound_;
    CheckpointInfo& cp_info_;
    ScreenFader screen_fader_;
    Player& player_;
    civilian::Florist& florist_;
    SpriteT& chair_left_;
    EventDetail detail_ = EventDetail::Stop;
    SceneObjects_Shop& objs_;
    struct TalkflowListener : ITalkflowListenerT {
        void on_flow_end(mgc_node_idx_t tag) override { 
            if ( tag == MGC_TALKSCRIPT_1_TAG_MESSAGE__END_COFFEE_BREAK ) {
                is_finished_ = true;
            }
        }
        void clear() { is_finished_ = false; }
        bool is_finished() const { return is_finished_; }
    private:
        bool is_finished_ = false;
    } talkflow_listener_;
};

}// namespace app

#endif/*MGC_EVENT_SHOP_2_HPP*/

