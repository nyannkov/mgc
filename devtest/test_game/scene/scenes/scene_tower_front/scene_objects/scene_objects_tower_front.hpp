#ifndef MGC_SCENE_OBJECTS_TOWER_FRONT_HPP
#define MGC_SCENE_OBJECTS_TOWER_FRONT_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/civilian/hen/hen.hpp"
#include "entity/civilian/chick/chick.hpp"
#include "entity/prop/entrance/entrance.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {
struct SceneObjects_TowerFront : ISceneObjects {

    explicit SceneObjects_TowerFront(SceneContext& scx) 
        : cp_info_(scx.checkpoint_info),
          props_ { &entrance_ },
          hen_(scx.gamepad),
          chick_(scx.gamepad),
          civils_ { &hen_, &chick_ } {
    }

    void init() override {
        hen_.spawn({MGC_CELL2PIXEL(54), MGC_CELL2PIXEL(11)-10}, civilian::HenAnimState::LookUp_Right);

        if ( cp_info_.has_reached(Checkpoint::TowerFrontEvent_Finished) ) {
            hen_.set_talkflow_request({
                &talkscript_1,
                MGC_TALKSCRIPT_1_TOWER_FRONT_TALK_3,
                nullptr,
                TalkflowEffectType::Default
            });
        }

        chick_.spawn({MGC_CELL2PIXEL(55)-8, MGC_CELL2PIXEL(11)-10}, civilian::ChickAnimState::LookUp_Right);

        entrance_.spawn({MGC_CELL2PIXEL(58), MGC_CELL2PIXEL(8)}, prop::EntranceType::Type1);
        entrance_.set_scene_transition_request( { SceneId::Corridor });
        if ( !cp_info_.has_reached(Checkpoint::TowerFrontEvent_Finished) ) {
            entrance_.set_visible(false);
        }
    }

    ArrayViewer<prop::Prop*> props() override { return { props_.data(), props_.size() }; }
    ArrayViewer<civilian::Civilian*> civils() override { return { civils_.data(), civils_.size() }; }
    ArrayViewer<item::Item*> items() override { return { }; }
    ArrayViewer<enemy::Enemy*> enemies() override { return { }; }
    ArrayViewer<block::Block*> blocks() override { return {}; }

    auto& entrance() { return entrance_; }
    auto& hen() { return hen_; }
    auto& chick() { return chick_; }

private:
    CheckpointInfo& cp_info_;

    // Props
    prop::Entrance entrance_;
    std::array<prop::Prop*, 1> props_;

    // Civilians
    civilian::Hen hen_;
    civilian::Chick chick_;
    std::array<civilian::Civilian*, 2> civils_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_TOWER_FRONT_HPP

