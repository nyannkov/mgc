#include "scene_objects_stage_3_4.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage3_4::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(57), MGC_CELL2PIXEL(59)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage3_2 }
    );

    bblock_1_.spawn({MGC_CELL2PIXEL(48), MGC_CELL2PIXEL(40)});
    bblock_2_.spawn({MGC_CELL2PIXEL(49), MGC_CELL2PIXEL(40)});
    bblock_3_.spawn({MGC_CELL2PIXEL(50), MGC_CELL2PIXEL(40)});

    gate_2_.spawn({MGC_CELL2PIXEL(39), MGC_CELL2PIXEL(30)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage3_5 }
    );
//    signboard_.spawn({MGC_CELL2PIXEL(38), MGC_CELL2PIXEL(30)});
//    signboard_.set_talkflow_request({
//        &talkscript_1,
//        MGC_TALKSCRIPT_1_PREPARING,
//        nullptr,
//        TalkflowEffectType::Mute
//    });
}


} // namespace app

