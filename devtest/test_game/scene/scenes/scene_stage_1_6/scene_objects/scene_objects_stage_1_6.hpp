#ifndef MGC_SCENE_OBJECTS_STAGE_1_6_HPP
#define MGC_SCENE_OBJECTS_STAGE_1_6_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/enemy/boss/dancer/dancer.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage1_6 : ISceneObjects {

    explicit SceneObjects_Stage1_6(SceneContext& scx, stage::Stage& stage)
        : cp_info_(scx.world_state.checkpoint_info),
          stage_(stage),
          dancer_(scx.timer, scx.sound),
          walker_ { 
            enemy::Walker(scx.timer, scx.sound),
            enemy::Walker(scx.timer, scx.sound),
            enemy::Walker(scx.timer, scx.sound),
            enemy::Walker(scx.timer, scx.sound),
            enemy::Walker(scx.timer, scx.sound),
            enemy::Walker(scx.timer, scx.sound)
          },
          enemies_ { 
            &walker_[0],
            &walker_[1],
            &walker_[2],
            &walker_[3],
            &walker_[4],
            &walker_[5],
            &dancer_
          },
          blocks_ { &block_1_, &block_2_ },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
//          signboard_(scx.gamepad),
          props_ { &gate_1_, &gate_2_} //, &signboard_ }
        { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size() }; }
    ArrayViewer<block::Block*> blocks() override { return { blocks_.data(), blocks_.size() }; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

    auto& walker() { return walker_; }
    auto& dancer() { return dancer_; }
    auto& stage() { return stage_; }

private:
    CheckpointInfo& cp_info_;
    stage::Stage& stage_;

    // Enemy
    std::array<enemy::Walker, 6> walker_;
    enemy::Dancer dancer_;
    std::array<enemy::Enemy*, 7> enemies_;

    // Block
    block::PushableBlock block_1_;
    block::PushableBlock block_2_;
    std::array<block::Block*, 2> blocks_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
//    prop::Signboard signboard_;
//    std::array<prop::Prop*, 3> props_;
    std::array<prop::Prop*, 2> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_1_6_HPP

