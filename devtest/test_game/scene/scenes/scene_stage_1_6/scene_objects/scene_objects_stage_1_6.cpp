#include "scene_objects_stage_1_6.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage1_6::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(22)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage1_5 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(33), MGC_CELL2PIXEL(2)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.close();

    signboard_.spawn({MGC_CELL2PIXEL(32), MGC_CELL2PIXEL(2)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

}


} // namespace app

