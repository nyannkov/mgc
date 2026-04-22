#ifndef MGC_SCENE_OBJECTS_STAGE_2_2_HPP
#define MGC_SCENE_OBJECTS_STAGE_2_2_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/enemy/floaty_fish/floaty_fish.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage2_2 : ISceneObjects {

    explicit SceneObjects_Stage2_2(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          skyfish_1_(scx.timer, scx.sound, scx.player),
          floaty_fish_1_(scx.timer, scx.sound), 
          floaty_fish_2_(scx.timer, scx.sound), 
          floaty_fish_3_(scx.timer, scx.sound), 
          floaty_fish_4_(scx.timer, scx.sound), 
          floaty_fish_5_(scx.timer, scx.sound), 
          floaty_fish_6_(scx.timer, scx.sound),
          enemies_ { 
            &skyfish_1_,
            &floaty_fish_1_, 
            &floaty_fish_2_, 
            &floaty_fish_3_, 
            &floaty_fish_4_, 
            &floaty_fish_5_, 
            &floaty_fish_6_
          },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          gate_3_(scx.gamepad),
          signboard_(scx.gamepad),
          props_ { &gate_1_, &gate_2_, &gate_3_, &signboard_ }
        { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size() }; }
    ArrayViewer<block::Block*> blocks() override { return {}; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    
private:
    CheckpointInfo& cp_info_;
    // Enemy
    enemy::SkyFish skyfish_1_;
    enemy::FloatyFish floaty_fish_1_;
    enemy::FloatyFish floaty_fish_2_;
    enemy::FloatyFish floaty_fish_3_;
    enemy::FloatyFish floaty_fish_4_;
    enemy::FloatyFish floaty_fish_5_;
    enemy::FloatyFish floaty_fish_6_;
    std::array<enemy::Enemy*, 7> enemies_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Gate gate_3_;
    prop::Signboard signboard_;
    std::array<prop::Prop*, 4> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_2_2_HPP

