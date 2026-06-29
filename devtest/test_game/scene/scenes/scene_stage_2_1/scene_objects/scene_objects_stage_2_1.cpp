#include "scene_objects_stage_2_1.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage2_1::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(30)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage1_7 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(5)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage2_2 }
    );

    gate_3_.spawn({MGC_CELL2PIXEL(57), MGC_CELL2PIXEL(30)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.close();
//    gate_3_.set_scene_transition_request(
//        { SceneId::Stage2_2 }
//    );
    signboard_.spawn({MGC_CELL2PIXEL(56), MGC_CELL2PIXEL(30)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

//    floaty_fish_1_.spawn({MGC_CELL2PIXEL(59), MGC_CELL2PIXEL(19)});

}


} // namespace app

