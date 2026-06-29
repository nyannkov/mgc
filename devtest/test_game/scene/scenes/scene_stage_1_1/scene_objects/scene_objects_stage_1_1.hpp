#ifndef MGC_SCENE_OBJECTS_STAGE_1_1_HPP
#define MGC_SCENE_OBJECTS_STAGE_1_1_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/prop/gate/gate.hpp"

namespace app {
struct SceneObjects_Stage1_1 : ISceneObjects {

    explicit SceneObjects_Stage1_1(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          walker_1_(scx.timer, scx.sound),
          walker_2_(scx.timer, scx.sound),
          walker_3_(scx.timer, scx.sound),
          enemies_ { &walker_1_, &walker_2_, &walker_3_},
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          props_ { &gate_1_, &gate_2_ } {
    }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }

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
    std::array<prop::Prop*, 2> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_1_1_HPP

