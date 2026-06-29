#include "scene_objects_stage_2_9.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage2_9::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(38), MGC_CELL2PIXEL(60)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage2_8 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(39), MGC_CELL2PIXEL(7)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.close();
//    gate_2_.set_scene_transition_request(
//        { SceneId::Stage3_1 }
//    );

    gate_3_.spawn({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(34)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.open();
    gate_3_.set_scene_transition_request(
        { SceneId::LogTerminal }
    );

    signboard_.spawn({MGC_CELL2PIXEL(40), MGC_CELL2PIXEL(7)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

}

} // namespace app

