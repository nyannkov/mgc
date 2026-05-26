#include "scene_objects_stage_2_7.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage2_7::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(8)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage2_6 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(53), MGC_CELL2PIXEL(2)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.close();
//    gate_2_.set_scene_transition_request(
//        { SceneId::Stage2_3 }
//    );

    gate_3_.spawn({MGC_CELL2PIXEL(67), MGC_CELL2PIXEL(8)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.close();
//    gate_3_.set_scene_transition_request(
//        { SceneId::Stage2_3 }
//    );

    button_.spawn({MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(2)});

    portal_1_.spawn({MGC_CELL2PIXEL(35), MGC_CELL2PIXEL(2)-8});

    signboard_.spawn({MGC_CELL2PIXEL(52), MGC_CELL2PIXEL(2)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });
    signboard2_.spawn({MGC_CELL2PIXEL(66), MGC_CELL2PIXEL(8)});
    signboard2_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });
    
    teleporters_[0].spawn(
        {MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(14)},
        {MGC_CELL2PIXEL(45), MGC_CELL2PIXEL(14)}
    );
    teleporters_[1].spawn(
        {MGC_CELL2PIXEL(45), MGC_CELL2PIXEL(14)},
        {MGC_CELL2PIXEL(19), MGC_CELL2PIXEL(14)}
    );
    teleporters_[2].spawn(
        {MGC_CELL2PIXEL(26), MGC_CELL2PIXEL(14)},
        {MGC_CELL2PIXEL(58), MGC_CELL2PIXEL(14)}
    );
    teleporters_[3].spawn(
        {MGC_CELL2PIXEL(58), MGC_CELL2PIXEL(14)},
        {MGC_CELL2PIXEL(26), MGC_CELL2PIXEL(14)}
    );
}


} // namespace app

