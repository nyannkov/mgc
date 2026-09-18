#ifndef MGC_SCENE_OBJECTS_STAGE_2_7_HPP
#define MGC_SCENE_OBJECTS_STAGE_2_7_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/enemy/floaty_fish/floaty_fish.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/prop/button/button.hpp"
#include "entity/prop/portal/portal.hpp"
#include "entity/prop/teleporter/teleporter.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage2_7 : ISceneObjects {

    explicit SceneObjects_Stage2_7(SceneContext& scx, stage::Stage& stage)
        : cp_info_(scx.world_state.checkpoint_info),
          stage_(stage),
          floaty_fish_1_(scx.timer, scx.sound, scx.player),
          enemies_ { &floaty_fish_1_ },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          gate_3_(scx.gamepad),
          portal_1_(scx.gamepad, scx.timer),
          signboard_(scx.gamepad),
          signboard2_(scx.gamepad),
          teleporters_ {
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad)
          },
          props_ { 
            &gate_1_,
            &gate_2_,
            &gate_3_,
            &button_,
            &portal_1_,
            &signboard_,
            &signboard2_,
            &teleporters_[0],
            &teleporters_[1],
            &teleporters_[2],
            &teleporters_[3]
          } { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size()}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

    auto& stage() { return stage_; }
    auto& button() { return button_; }
    auto& portal() { return portal_1_; }
    auto& teleporters() { return teleporters_; }

private:
    CheckpointInfo& cp_info_;
    stage::Stage& stage_;

    enemy::FloatyFish floaty_fish_1_;
    std::array<enemy::Enemy*, 1> enemies_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Gate gate_3_;
    prop::Button button_;
    prop::Portal portal_1_;
    prop::Signboard signboard_;
    prop::Signboard signboard2_;
    std::array<prop::Teleporter, 4> teleporters_;
    std::array<prop::Prop*, 11> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_2_7_HPP

