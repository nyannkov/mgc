#ifndef MGC_SCENE_OBJECTS_STAGE_3_2_HPP
#define MGC_SCENE_OBJECTS_STAGE_3_2_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/enemy/balloon/balloon.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"
#include "entity/carrier/waypoint_carrier/waypoint_carrier.hpp"

namespace app {
struct SceneObjects_Stage3_2 : ISceneObjects {

    explicit SceneObjects_Stage3_2(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          balloon_1_(scx.timer, scx.sound, scx.player),
          balloon_2_(scx.timer, scx.sound, scx.player),
          balloon_3_(scx.timer, scx.sound, scx.player),
          skyfish_1_(scx.timer, scx.sound, scx.player),
          skyfish_2_(scx.timer, scx.sound, scx.player),
          walker_1_(scx.timer, scx.sound),
          walker_2_(scx.timer, scx.sound),
          enemies_ {
            &balloon_1_,
            &balloon_2_,
            &balloon_3_, 
            &skyfish_1_,
            &skyfish_2_,
            &walker_1_,
            &walker_2_
          },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          signboard_(scx.gamepad),
          props_ { &gate_1_, &gate_2_, &signboard_ },
          carrier_1_(scx.timer),
          carrier_2_1_(scx.timer),
          carrier_2_2_(scx.timer),
          carrier_3_(scx.timer),
          carrier_4_(scx.timer),
          carriers_ { 
            &carrier_1_,
            &carrier_2_1_,
            &carrier_2_2_,
            &carrier_3_,
            &carrier_4_ 
         } { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size()}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return { carriers_.data(), carriers_.size() }; }
    
private:
    CheckpointInfo& cp_info_;

    // enemy
    enemy::Balloon balloon_1_;
    enemy::Balloon balloon_2_;
    enemy::Balloon balloon_3_;
    enemy::SkyFish skyfish_1_;
    enemy::SkyFish skyfish_2_;
    enemy::Walker walker_1_;
    enemy::Walker walker_2_;
    std::array<enemy::Enemy*, 7> enemies_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Signboard signboard_;
    std::array<prop::Prop*, 3> props_;

    std::array<carrier::WaypointT, 2> wp_1_ = {{
        {MGC_CELL2PIXEL(51), MGC_CELL2PIXEL(58)},
        {MGC_CELL2PIXEL(51), MGC_CELL2PIXEL(37)},
    }};
    carrier::WaypointCarrier<1, 3> carrier_1_;

    std::array<carrier::WaypointT, 6> wp_2_ = {{
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(39)},
        {MGC_CELL2PIXEL(36), MGC_CELL2PIXEL(39)},
        {MGC_CELL2PIXEL(36), MGC_CELL2PIXEL(19)},
        {MGC_CELL2PIXEL(50), MGC_CELL2PIXEL(19)},
        {MGC_CELL2PIXEL(50), MGC_CELL2PIXEL(15)},
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(15)},
    }};
    carrier::WaypointCarrier<1, 3> carrier_2_1_;
    carrier::WaypointCarrier<1, 3> carrier_2_2_;

    std::array<carrier::WaypointT, 2> wp_3_ = {{
        {MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(12)},
        {MGC_CELL2PIXEL(50), MGC_CELL2PIXEL(12)},
    }};
    carrier::WaypointCarrier<1, 3> carrier_3_;

    std::array<carrier::WaypointT, 2> wp_4_ = {{
        {MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(43)},
        {MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(58)},
    }};
    carrier::WaypointCarrier<1, 3> carrier_4_;

    std::array<carrier::Carrier*, 5> carriers_;

};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_3_2_HPP

