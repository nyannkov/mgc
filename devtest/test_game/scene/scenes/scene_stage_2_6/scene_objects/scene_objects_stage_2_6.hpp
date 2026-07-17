#ifndef MGC_SCENE_OBJECTS_STAGE_2_6_HPP
#define MGC_SCENE_OBJECTS_STAGE_2_6_HPP

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
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage2_6 : ISceneObjects {

    explicit SceneObjects_Stage2_6(SceneContext& scx, stage::Stage& stage)
        : cp_info_(scx.world_state.checkpoint_info),
          stage_(stage),
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          props_ { 
            &gate_1_,
            &gate_2_
          } { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

    auto& stage() { return stage_; }

private:
    CheckpointInfo& cp_info_;
    stage::Stage& stage_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    std::array<prop::Prop*, 2> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_2_6_HPP

