#ifndef MGC_SCENE_OBJECTS_STAGE_3_5_HPP
#define MGC_SCENE_OBJECTS_STAGE_3_5_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/enemy/balloon/balloon.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/prop/button/button.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"
#include "entity/carrier/waypoint_carrier/waypoint_carrier.hpp"

namespace app {
struct SceneObjects_Stage3_5 : ISceneObjects {

    explicit SceneObjects_Stage3_5(SceneContext& scx)
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
          gate_3_(scx.gamepad),
          signboard_(scx.gamepad),
          button_(prop::ButtonType::Type2),
          props_ { &gate_1_, &gate_2_, &gate_3_, &signboard_, &button_ },
          carrier_1_(scx.timer),
          carriers_ { 
            &carrier_1_,
         } { }

    void init();

    auto& button() { return button_; }
    auto& carrier() { return carrier_1_; }

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
    prop::Gate gate_3_;
    prop::Signboard signboard_;
    prop::Button button_;
    std::array<prop::Prop*, 5> props_;

    std::array<carrier::WaypointT, 10> wp_1_ = {{
        {MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(60)},
        {MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(48)},
        {MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(48)},
        {MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(56)},
        {MGC_CELL2PIXEL(52), MGC_CELL2PIXEL(56)},
        {MGC_CELL2PIXEL(52), MGC_CELL2PIXEL(47)},
        {MGC_CELL2PIXEL(26), MGC_CELL2PIXEL(47)},
        {MGC_CELL2PIXEL(26), MGC_CELL2PIXEL(33)},
        {MGC_CELL2PIXEL(54), MGC_CELL2PIXEL(33)},
        {MGC_CELL2PIXEL(54), MGC_CELL2PIXEL(25)},
    }};
    carrier::WaypointCarrier<1, 3> carrier_1_;
    std::array<carrier::Carrier*, 1> carriers_;

};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_3_5_HPP

