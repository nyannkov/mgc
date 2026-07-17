#include "scene_objects_stage_3_1.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage3_1::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(27)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage2_9 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(11)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.close();
//    gate_2_.open();
//    gate_2_.set_scene_transition_request(
//        { SceneId::Stage3_2 }
//    );

    signboard_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(11)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

//    carrier_.spawn({MGC_CELL2PIXEL(10)+8, MGC_CELL2PIXEL(22)});
}


} // namespace app

