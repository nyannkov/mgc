#ifndef MGC_SCENE_OBJECTS_STAGE_2_8_HPP
#define MGC_SCENE_OBJECTS_STAGE_2_8_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/floaty_fish/floaty_fish.hpp"
#include "entity/enemy/boss/lancer/lancer.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"
#include "entity/prop/button/button.hpp"

namespace app {
struct SceneObjects_Stage2_8 : ISceneObjects {

    explicit SceneObjects_Stage2_8(SceneContext& scx, stage::Stage& stage)
        : cp_info_(scx.world_state.checkpoint_info),
          stage_(stage),
          lancer_(scx.timer, scx.sound, scx.player),
          floaty_ { 
            enemy::FloatyFish(scx.timer, scx.sound, scx.player),
            enemy::FloatyFish(scx.timer, scx.sound, scx.player),
            enemy::FloatyFish(scx.timer, scx.sound, scx.player),
            enemy::FloatyFish(scx.timer, scx.sound, scx.player),
            enemy::FloatyFish(scx.timer, scx.sound, scx.player),
            enemy::FloatyFish(scx.timer, scx.sound, scx.player)
          },
          enemies_ { 
            &floaty_[0],
            &floaty_[1],
            &floaty_[2],
            &floaty_[3],
            &floaty_[4],
            &floaty_[5],
            &lancer_
          },
          blocks_ { &block_1_, &block_2_ },
          gate_1_(scx.gamepad),
          gate_2_(scx.gamepad),
          button_(prop::ButtonType::Type3),
          props_ { &gate_1_, &gate_2_, &button_}
        { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size() }; }
    ArrayViewer<block::Block*> blocks() override { return { blocks_.data(), blocks_.size() }; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }

    auto& floaty() { return floaty_; }
    auto& lancer() { return lancer_; }
    auto& stage() { return stage_; }

    void draw(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override {
        for ( auto* civil : civils() ) {
            civil->draw(fb, cam_pos);
        }
        for ( auto* prop : props() ) {
            prop->draw(fb, cam_pos);
        }
        for ( auto* item : items() ) {
            item->draw(fb, cam_pos);
        }
        for ( auto* block : blocks() ) {
            block->draw(fb, cam_pos);
        }
    }

    void draw_before(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override { 
        for ( auto* enemy : enemies() ) {
            enemy->draw(fb, cam_pos);
            for ( auto* weapon : enemy->weapons() ) {
                weapon->draw(fb, cam_pos);
            }
        }
    }

private:
    CheckpointInfo& cp_info_;
    stage::Stage& stage_;

    // Enemy
    std::array<enemy::FloatyFish, 6> floaty_;
    enemy::Lancer lancer_;
    std::array<enemy::Enemy*, 7> enemies_;

    // Block
    block::PushableBlock block_1_;
    block::PushableBlock block_2_;
    std::array<block::Block*, 2> blocks_;

    // Prop
    prop::Gate gate_1_;
    prop::Gate gate_2_;
    prop::Button button_;
//    prop::Signboard signboard_;
//    std::array<prop::Prop*, 3> props_;
    std::array<prop::Prop*, 3> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_2_8_HPP



