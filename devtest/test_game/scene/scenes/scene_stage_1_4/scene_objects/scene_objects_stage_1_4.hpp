#ifndef MGC_SCENE_OBJECTS_STAGE_1_4_HPP
#define MGC_SCENE_OBJECTS_STAGE_1_4_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/item/potion/potion.hpp"
#include "entity/item/elixir/elixir.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/teleporter/teleporter.hpp"
#include "entity/prop/portal/portal.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage1_4 : ISceneObjects {

    explicit SceneObjects_Stage1_4(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          walker_1_(scx.timer, scx.sound),
          enemies_ { &walker_1_ },
          gate_1_(scx.gamepad),
          portal_1_(scx.gamepad, scx.timer),
          teleporters_ {
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad)
          },
          props_ { 
            &gate_1_,
            &portal_1_,
            &teleporters_[0],
            &teleporters_[1],
            &teleporters_[2],
            &teleporters_[3],
            &teleporters_[4],
            &teleporters_[5],
            &teleporters_[6],
            &teleporters_[7],
            &teleporters_[8],
            &teleporters_[9],
            &teleporters_[10],
            &teleporters_[11],
            &teleporters_[12],
            &teleporters_[13],
            &teleporters_[14],
            &teleporters_[15],
            &teleporters_[16],
            &teleporters_[17],
            &teleporters_[18],
            &teleporters_[19]
          } { 
     }

    void init();

    ArrayViewer<prop::Prop*> props() override {
        return { props_.data(), props_.size() };
    }

    ArrayViewer<enemy::Enemy*> enemies() override {
        return { enemies_.data(), enemies_.size() };
    }

    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }

    auto& teleporters() { return teleporters_; }
    auto& portal() { return portal_1_; }
    
private:
    CheckpointInfo& cp_info_;

    // Enemy
    enemy::Walker walker_1_;
    std::array<enemy::Enemy*, 1> enemies_;

    // Prop
    prop::Gate gate_1_;
    prop::Portal portal_1_;
    std::array<prop::Teleporter, 20> teleporters_;
    std::array<prop::Prop*, 22> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_1_4_HPP

