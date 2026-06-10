#ifndef MGC_SCENE_OBJECTS_STAGE_2_8_HPP
#define MGC_SCENE_OBJECTS_STAGE_2_8_HPP

#include <utility>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/enemy/skyfish/skyfish.hpp"
#include "entity/enemy/walker/walker.hpp"
#include "entity/enemy/boss/lancer/lancer.hpp"
#include "entity/prop/gate/gate.hpp"
#include "entity/prop/signboard/signboard.hpp"
#include "entity/block/pushable_block/pushable_block.hpp"

namespace app {
struct SceneObjects_Stage2_8 : ISceneObjects {

    explicit SceneObjects_Stage2_8(SceneContext& scx, stage::Stage& stage)
        : cp_info_(scx.world_state.checkpoint_info),
          stage_(stage),
          lancer_(scx.timer, scx.sound, scx.player),
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
            &lancer_
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

    auto& walker() { return walker_; }
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
    std::array<enemy::Walker, 6> walker_;
    enemy::Lancer lancer_;
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

#endif // MGC_SCENE_OBJECTS_STAGE_2_8_HPP



