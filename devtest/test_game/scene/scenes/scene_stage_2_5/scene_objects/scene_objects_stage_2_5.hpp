#ifndef MGC_SCENE_OBJECTS_STAGE_2_5_HPP
#define MGC_SCENE_OBJECTS_STAGE_2_5_HPP

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
struct SceneObjects_Stage2_5 : ISceneObjects {

    explicit SceneObjects_Stage2_5(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          walker_1_(scx.timer, scx.sound),
          skyfish_1_(scx.timer, scx.sound, scx.player),
          floaty_fish_1_(scx.timer, scx.sound, scx.player),
          enemies_ { &walker_1_, &skyfish_1_, &floaty_fish_1_ },
          blocks_ { &block_1_, &block_2_ },
          gate_1_(scx.gamepad),
          buttons_ {
            prop::Button(prop::ButtonType::Type2),
            prop::Button(prop::ButtonType::Type2),
            prop::Button(prop::ButtonType::Type2),
            prop::Button(prop::ButtonType::Type2),
            prop::Button(prop::ButtonType::Type2)
          },
          props_ { 
            &gate_1_,
            &buttons_[0],
            &buttons_[1],
            &buttons_[2],
            &buttons_[3],
            &buttons_[4]
          } { }

    void init();

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { enemies_.data(), enemies_.size() }; }
    ArrayViewer<block::Block*> blocks() override { return { blocks_.data(), blocks_.size() }; }
    ArrayViewer<civilian::Civilian*> civils() override { return {}; }
    ArrayViewer<carrier::Carrier*> carriers() override { return {}; }

    auto& buttons() { return buttons_; }
    auto& label_counter() { return label_counter_; }
    void draw(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) override {

        ISceneObjects::draw(fb, cam_pos);
        label_counter_.draw(fb, cam_pos);
    }

private:
    CheckpointInfo& cp_info_;
    LabelT label_counter_;
    // Enemy
    enemy::SkyFish skyfish_1_;
    enemy::Walker walker_1_;
    enemy::FloatyFish floaty_fish_1_;
    std::array<enemy::Enemy*, 3> enemies_;

    // Block
    block::PushableBlock block_1_;
    block::PushableBlock block_2_;
    std::array<block::Block*, 2> blocks_;

    // Prop
    prop::Gate gate_1_;
    std::array<prop::Button, 5> buttons_;
    std::array<prop::Prop*, 6> props_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_STAGE_2_5_HPP

