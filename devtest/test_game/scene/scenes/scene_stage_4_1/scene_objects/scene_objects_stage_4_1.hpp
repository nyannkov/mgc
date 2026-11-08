#ifndef MGC_SCENE_OBJECTS_STAGE_4_1_HPP
#define MGC_SCENE_OBJECTS_STAGE_4_1_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/item/potion/potion.hpp"
#include "entity/item/elixir/elixir.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/teleporter/teleporter.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage4_1 : ISceneObjects {

    explicit SceneObjects_Stage4_1(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          walker_1_(scx.timer, scx.sound),
          enemies_ { &walker_1_ },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          gate_3_(scx.gamepad),
          gate_4_(scx.gamepad),
          props_ { 
            &gate_1_,
            &gate_2_,
            &gate_3_,
            &gate_4_,
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
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

private:
    CheckpointInfo& cp_info_;

    // Enemy
    enemy::Walker walker_1_;
    std::array<enemy::Enemy*, 1> enemies_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Gate gate_3_;
    prop::Gate gate_4_;
    std::array<prop::Prop*, 4> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_4_1_HPP

