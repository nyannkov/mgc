#include "scene_objects_stage_3_5.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage3_5::init() {

//    balloon_1_.spawn({MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(11)}, true);
//    balloon_2_.spawn({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(37)}, true);
//    balloon_3_.spawn({MGC_CELL2PIXEL(42), MGC_CELL2PIXEL(54)}, false);
//
//    walker_1_.spawn({MGC_CELL2PIXEL(14), MGC_CELL2PIXEL(60)}, true);
//    walker_2_.spawn({MGC_CELL2PIXEL(44), MGC_CELL2PIXEL(60)}, false);
//
//    skyfish_1_.spawn({MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(36)}, true);
//    skyfish_2_.spawn({MGC_CELL2PIXEL(16), MGC_CELL2PIXEL(11)}, false);

    gate_1_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(60)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage3_4 }
    );

    button_.spawn({MGC_CELL2PIXEL(35), MGC_CELL2PIXEL(39)});

    gate_2_.spawn({MGC_CELL2PIXEL(12), MGC_CELL2PIXEL(35)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::LogTerminal }
    );

    gate_3_.spawn({MGC_CELL2PIXEL(58), MGC_CELL2PIXEL(14)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.close();
//    gate_3_.open();
//    gate_3_.set_scene_transition_request(
//        { SceneId::Stage3_4 }
//    );
    signboard_.spawn({MGC_CELL2PIXEL(57), MGC_CELL2PIXEL(14)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

    carrier_1_.spawn(wp_1_.data()[0]);
    carrier_1_.start(wp_1_.data(), wp_1_.size(), carrier::WaypointCarrierMode::OneWay);
}


} // namespace app

