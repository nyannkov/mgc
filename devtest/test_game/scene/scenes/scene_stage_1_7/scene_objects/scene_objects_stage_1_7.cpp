#include "scene_objects_stage_1_7.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage1_7::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(60)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage1_6 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(7)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage2_1 }
    );
}


} // namespace app

