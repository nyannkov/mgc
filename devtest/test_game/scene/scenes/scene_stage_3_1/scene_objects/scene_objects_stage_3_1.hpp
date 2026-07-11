#ifndef MGC_SCENE_OBJECTS_STAGE_3_1_HPP
#define MGC_SCENE_OBJECTS_STAGE_3_1_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage3_1 : ISceneObjects {

    explicit SceneObjects_Stage3_1(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          signboard_(scx.gamepad),
          props_ { &gate_1_, &gate_2_, &signboard_ }
        { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    
private:
    CheckpointInfo& cp_info_;
    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Signboard signboard_;
    std::array<prop::Prop*, 3> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_3_1_HPP

