#ifndef MGC_SCENE_OBJECTS_SHOP_HPP
#define MGC_SCENE_OBJECTS_SHOP_HPP

#include <stdio.h>
#include "mgc_cpp/mgc.hpp"
#include "scene/scene_context.hpp"
#include "scene/interface/iscene_objects.hpp"
#include "entity/prop/door/door.hpp"
#include "entity/prop/portal/portal.hpp"
#include "entity/civilian/florist/florist.hpp"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/mml/mml.h"

namespace app {


struct SceneObjects_Shop : ISceneObjects {

    explicit SceneObjects_Shop(SceneContext& scx)
        : cp_info_(scx.world_state.checkpoint_info),
          door_(scx.gamepad),
          portal_(scx.gamepad, scx.timer),
          props_ { &door_, &portal_ },
          florist_(
            scx.gamepad, 
            scx.world_state.checkpoint_info,
            scx.player
          ),
          civils_ {&florist_ } {
    }

    void init() override {
        door_.spawn({MGC_CELL2PIXEL(16), MGC_CELL2PIXEL(5)+8}, false);
        door_.lock();
        door_.set_talkflow_request({
            &talkscript_1,
            MGC_TALKSCRIPT_1_GATE_1,
            nullptr,
            TalkflowEffectType::Mute
        });

        portal_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(5)+8});

        florist_.spawn(
            {MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(5)+8},
            civilian::FloristAnimState::Stand_Left);

    }

    ArrayViewer<prop::Prop*> props() override {
        return { props_.data(), props_.size() };
    }
    ArrayViewer<civilian::Civilian*> civils() override {
        return { civils_.data(), civils_.size() };
    }
    ArrayViewer<enemy::Enemy*> enemies() override { return {}; }
    ArrayViewer<item::Item*> items() override { return {}; }
    ArrayViewer<block::Block*> blocks() override { return {}; }

    auto& door() { return door_; }
    auto& portal() { return portal_; }

private:
    CheckpointInfo& cp_info_;
    prop::Door door_;
    prop::Portal portal_;
    std::array<prop::Prop*, 2> props_;
    civilian::Florist florist_;
    std::array<civilian::Civilian*, 1> civils_;
};

} // namespace app

#endif // MGC_SCENE_OBJECTS_SHOP_HPP

