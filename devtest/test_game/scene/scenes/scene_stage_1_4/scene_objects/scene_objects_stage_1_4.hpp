#ifndef MGC_SCENE_OBJECTS_STAGE_1_4_HPP
#define MGC_SCENE_OBJECTS_STAGE_1_4_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/item/potion/potion.hpp"
#include "entity/item/elixir/elixir.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/teleporter/teleporter.hpp"
#include "entity/prop/portal/portal.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage1_4 : ISceneObjects {

    explicit SceneObjects_Stage1_4(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          skyfish_1_(scx.timer, scx.sound, scx.player),
          skyfish_2_(scx.timer, scx.sound, scx.player),
          skyfish_3_(scx.timer, scx.sound, scx.player),
          enemies_ { &skyfish_1_, &skyfish_2_, &skyfish_3_ },
          items_ { &potion_, &elixir_ },
          gate_1_(scx.gamepad),
          portal_1_(scx.gamepad, scx.timer),
          teleporters_ {
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad),
            prop::Teleporter(scx.gamepad)
          },
          props_ { 
            &gate_1_,
            &portal_1_,
            &teleporters_[0],
            &teleporters_[1],
            &teleporters_[2],
            &teleporters_[3],
            &teleporters_[4],
            &teleporters_[5],
            &teleporters_[6],
            &teleporters_[7],
            &teleporters_[8],
            &teleporters_[9],
            &teleporters_[10],
            &teleporters_[11],
            &teleporters_[12],
            &teleporters_[13],
            &teleporters_[14],
            &teleporters_[15]
          } { }

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

    ArrayViewer<block::Block*> blocks() override { return {}; }

    ArrayViewer<civilian::Civilian*> civils() override { return {}; }

    auto& teleporters() { return teleporters_; }
    auto& portal() { return portal_1_; }
    
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
    prop::Portal portal_1_;
    std::array<prop::Teleporter, 16> teleporters_;
    std::array<prop::Prop*, 18> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_1_4_HPP

