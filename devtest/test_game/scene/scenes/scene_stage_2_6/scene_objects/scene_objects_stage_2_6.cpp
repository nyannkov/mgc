#include "scene_objects_stage_2_6.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage2_6::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(11)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage2_3 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(157), MGC_CELL2PIXEL(11)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.close();
//    gate_2_.set_scene_transition_request(
//        { SceneId::Stage2_3 }
//    );

    signboard_.spawn({MGC_CELL2PIXEL(156), MGC_CELL2PIXEL(11)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });
}


} // namespace app

