#ifndef MGC_SCENE_OBJECTS_STAGE_1_3_HPP
#define MGC_SCENE_OBJECTS_STAGE_1_3_HPP

#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/item/potion/potion.hpp"
#include "entity/item/elixir/elixir.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage1_3 : ISceneObjects {

    explicit SceneObjects_Stage1_3(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          skyfish_1_(scx.timer, scx.sound, scx.player),
          skyfish_2_(scx.timer, scx.sound, scx.player),
          skyfish_3_(scx.timer, scx.sound, scx.player),
          enemies_ { &skyfish_1_, &skyfish_2_, &skyfish_3_ },
          items_ { &potion_, &elixir_ },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          gate_3_(scx.gamepad),
          signboard_(scx.gamepad),
          props_ { &gate_1_, &gate_2_, &gate_3_, &signboard_ },
          blocks_{ &pushable_block_, &pushable_block2_, &pushable_block3_, &pushable_block4_ } {
    }

    void init();

    ArrayViewer<prop::Prop*> props() override {
        return { props_.data(), props_.size() };
    }

    ArrayViewer<item::Item*> items() override {
        return { items_.data(), items_.size() };
    }

    ArrayViewer<enemy::Enemy*> enemies() override {
        return { enemies_.data(), enemies_.size() };
    }

    ArrayViewer<block::Block*> blocks() override { 
        return { blocks_.data(), blocks_.size() };
    }

    ArrayViewer<civilian::Civilian*> civils() override { return {}; }

private:
    CheckpointInfo& cp_info_;

    // Enemy
    enemy::SkyFish skyfish_1_;
    enemy::SkyFish skyfish_2_;
    enemy::SkyFish skyfish_3_;
    std::array<enemy::Enemy*, 3> enemies_;

    // Item
    item::Potion potion_;
    item::Elixir elixir_;
    std::array<item::Item*, 2> items_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Gate gate_3_;
    prop::Signboard signboard_;
    std::array<prop::Prop*, 4> props_;

    // Block
    block::PushableBlock pushable_block_{};
    block::PushableBlock pushable_block2_{};
    block::PushableBlock pushable_block3_{};
    block::PushableBlock pushable_block4_{};
    std::array<block::Block*, 4> blocks_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_1_3_HPP

