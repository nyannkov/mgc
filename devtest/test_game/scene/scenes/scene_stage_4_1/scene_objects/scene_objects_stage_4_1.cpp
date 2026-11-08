#include "scene_objects_stage_4_1.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage4_1::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(28), MGC_CELL2PIXEL(30)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage3_7 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(13)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage4_2, 2 }
    );

    gate_3_.spawn({MGC_CELL2PIXEL(28), MGC_CELL2PIXEL(2)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.open();
    gate_3_.set_scene_transition_request(
        { SceneId::Stage4_2, 3 }
    );

    gate_4_.spawn({MGC_CELL2PIXEL(4), MGC_CELL2PIXEL(3)});
    gate_4_.set_gate_type(prop::GateType::Type2);
    gate_4_.open();
    gate_4_.set_scene_transition_request(
        { SceneId::Stage4_2, 4 }
    );

    //walker_1_.spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(9)}, true);
}


} // namespace app

