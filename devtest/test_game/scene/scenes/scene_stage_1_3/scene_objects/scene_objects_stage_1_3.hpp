#ifndef MGC_SCENE_OBJECTS_STAGE_1_3_HPP
#define MGC_SCENE_OBJECTS_STAGE_1_3_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/item/potion/potion.hpp"
#include "entity/item/elixir/elixir.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/button/button.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage1_3 : ISceneObjects {

    explicit SceneObjects_Stage1_3(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          walker_1_(scx.timer, scx.sound),
          walker_2_(scx.timer, scx.sound),
          walker_3_(scx.timer, scx.sound),
          enemies_ { &walker_1_, &walker_2_, &walker_3_ },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          gate_3_(scx.gamepad),
          props_ { &gate_1_, &gate_2_, &gate_3_, &button_ },
          blocks_ { &pushable_block_ } {
    }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size() }; }
    ArrayViewer<block::Block*> blocks() override { return { blocks_.data(), blocks_.size() }; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

    auto& button() { return button_; }
    auto& locked_gate() { return gate_3_; }

private:
    CheckpointInfo& cp_info_;

    // Enemy
    enemy::Walker walker_1_;
    enemy::Walker walker_2_;
    enemy::Walker walker_3_;
    std::array<enemy::Enemy*, 3> enemies_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Gate gate_3_;
    prop::Button button_;
    std::array<prop::Prop*, 4> props_;

    // Block
    block::PushableBlock pushable_block_{};
    std::array<block::Block*, 1> blocks_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_1_3_HPP

