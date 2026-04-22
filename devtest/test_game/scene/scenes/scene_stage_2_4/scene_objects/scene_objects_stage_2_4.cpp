#include "scene_objects_stage_2_4.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage2_4::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(2)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage2_2 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(13), MGC_CELL2PIXEL(2)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage2_3 }
    );

    gate_3_.spawn({MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(30)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.close();
//    gate_3_.set_scene_transition_request(
//        { SceneId::Stage2_3 }
//    );
    signboard_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(30)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

    portal_1_.spawn({MGC_CELL2PIXEL(13), MGC_CELL2PIXEL(30)-8});

//    floaty_fish_1_.spawn({MGC_CELL2PIXEL(59), MGC_CELL2PIXEL(19)});

}


} // namespace app

