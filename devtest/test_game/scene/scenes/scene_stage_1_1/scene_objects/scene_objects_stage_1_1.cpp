#include "scene_objects_stage_1_1.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage1_1::init() {

    walker_1_.spawn({MGC_CELL2PIXEL(13), MGC_CELL2PIXEL(30)}, true);
    walker_2_.spawn({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(17)}, true);
    walker_3_.spawn({MGC_CELL2PIXEL(7), MGC_CELL2PIXEL(5)}, true);
    
    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(30)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Corridor }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(5)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage1_2 }
    );
}


} // namespace app

