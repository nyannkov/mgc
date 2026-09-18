#ifndef MGC_SCENE_OBJECTS_STAGE_3_3_HPP
#define MGC_SCENE_OBJECTS_STAGE_3_3_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/enemy/balloon/balloon.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/prop/portal/portal.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"
#include "entity/block/breakable_block/breakable_block.hpp"
#include "entity/carrier/waypoint_carrier/waypoint_carrier.hpp"

namespace app {
struct SceneObjects_Stage3_3 : ISceneObjects {

    explicit SceneObjects_Stage3_3(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          gate_1_(scx.gamepad),
          portal_1_(scx.gamepad, scx.timer),
          props_ { &gate_1_, &portal_1_ }
          { }

    void init();
    auto& portal() { return portal_1_; }

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return {}; } 
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }
    
private:
    CheckpointInfo& cp_info_;

    // Prop
    prop::Gate gate_1_;
    prop::Portal portal_1_;
    std::array<prop::Prop*, 2> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_3_3_HPP

