#ifndef MGC_SCENE_OBJECTS_STAGE_3_4_HPP
#define MGC_SCENE_OBJECTS_STAGE_3_4_HPP

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
struct SceneObjects_Stage3_4 : ISceneObjects {

    explicit SceneObjects_Stage3_4(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          signboard_(scx.gamepad),
          props_ { &gate_1_, &gate_2_, &signboard_ },
         bblock_1_(scx.timer),
         bblock_2_(scx.timer),
         bblock_3_(scx.timer),
         blocks_ {
            &bblock_1_,
            &bblock_2_,
            &bblock_3_
         }
         { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return { blocks_.data(), blocks_.size() }; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; } 
    
private:
    CheckpointInfo& cp_info_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Signboard signboard_;
    std::array<prop::Prop*, 3> props_;

    // Block
    block::BreakableBlock bblock_1_;
    block::BreakableBlock bblock_2_;
    block::BreakableBlock bblock_3_;
    std::array<block::Block*, 3> blocks_;

};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_3_4_HPP

