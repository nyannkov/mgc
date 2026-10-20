#include "scene_objects_stage_3_6.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage3_6::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(22)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage3_5 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(37), MGC_CELL2PIXEL(22)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.close();
//    gate_2_.set_scene_transition_request(
//        { SceneId::Stage3_7 }
//    );

    mic_1_.spawn({MGC_CELL2PIXEL(20)-3, MGC_CELL2PIXEL(11)});

    signboard_.spawn({MGC_CELL2PIXEL(36), MGC_CELL2PIXEL(22)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

    spotlight1_.set_position({MGC_CELL2PIXEL(25)+8, MGC_CELL2PIXEL(2)});
    spotlight1_.set_delta1({MGC_CELL2PIXEL(-3), MGC_CELL2PIXEL(11)});
    spotlight1_.set_delta2({MGC_CELL2PIXEL(-9), MGC_CELL2PIXEL(11)});
    spotlight1_.set_color(MGC_COLOR(1.0, 1.0, 1.0));
    spotlight1_.set_alpha(50);
    spotlight1_.turn_off();

    spotlight2_.set_position({MGC_CELL2PIXEL(13)+8, MGC_CELL2PIXEL(2)});
    spotlight2_.set_delta1({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(11)});
    spotlight2_.set_delta2({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(11)});
    spotlight2_.set_color(MGC_COLOR(1.0, 1.0, 1.0));
    spotlight2_.set_alpha(50);
    spotlight2_.turn_off();
}


} // namespace app

