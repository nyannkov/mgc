#include "scene_objects_stage_1_5.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage1_5::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(62)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage1_3 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(29), MGC_CELL2PIXEL(6)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.close();

    signboard_.spawn({MGC_CELL2PIXEL(28), MGC_CELL2PIXEL(6)});
    signboard_.set_talkflow_request({
        &talkscript_1,
        MGC_TALKSCRIPT_1_PREPARING,
        nullptr,
        TalkflowEffectType::Mute
    });

    walker_1_.spawn({MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(62)}, true);
    skyfish_1_.spawn({MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(17)}, true);

    block_1_.spawn({MGC_CELL2PIXEL(21), MGC_CELL2PIXEL(43)});
    block_2_.spawn({MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(33)});
}


} // namespace app

