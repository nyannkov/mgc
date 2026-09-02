#include "scene_objects_stage_3_2.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage3_2::init() {

    balloon_1_.spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(11)}, true);
    balloon_2_.spawn({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(37)}, true);
    balloon_3_.spawn({MGC_CELL2PIXEL(42), MGC_CELL2PIXEL(54)}, false);

    walker_1_.spawn({MGC_CELL2PIXEL(14), MGC_CELL2PIXEL(60)}, true);
    walker_2_.spawn({MGC_CELL2PIXEL(44), MGC_CELL2PIXEL(60)}, false);

    skyfish_1_.spawn({MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(36)}, true);
    skyfish_2_.spawn({MGC_CELL2PIXEL(16), MGC_CELL2PIXEL(11)}, false);

    gate_1_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(60)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage3_1 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(59), MGC_CELL2PIXEL(10)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.close();
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage3_3 }
    );

    gate_3_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(14)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.close();
//    gate_3_.open();
//    gate_3_.set_scene_transition_request(
//        { SceneId::Stage3_3 }
//    );
    signboard_.spawn({MGC_CELL2PIXEL(5), MGC_CELL2PIXEL(14)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

    carrier_1_.spawn(wp_1_.data()[0]);
    carrier_1_.start(wp_1_.data(), wp_1_.size(), carrier::WaypointCarrierMode::Loop);

    carrier_2_1_.spawn(wp_2_.data()[0]);
    carrier_2_1_.start(wp_2_.data(), wp_2_.size(), carrier::WaypointCarrierMode::Loop);

    carrier_2_2_.spawn(wp_2_.data()[3]);
    carrier_2_2_.start(wp_2_.data(), wp_2_.size(), carrier::WaypointCarrierMode::Loop, 3);

    carrier_3_.spawn(wp_3_.data()[0]);
    carrier_3_.start(wp_3_.data(), wp_3_.size(), carrier::WaypointCarrierMode::Loop);

    carrier_4_.spawn(wp_4_.data()[0]);
    carrier_4_.start(wp_4_.data(), wp_4_.size(), carrier::WaypointCarrierMode::Loop);
}


} // namespace app

