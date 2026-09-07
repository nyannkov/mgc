#include "scene_objects_stage_3_3.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage3_3::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(49)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage3_2 }
    );

    portal_1_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(15)-8});
}


} // namespace app

