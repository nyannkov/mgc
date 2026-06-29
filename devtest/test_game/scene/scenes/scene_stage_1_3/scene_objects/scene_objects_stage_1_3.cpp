#include "scene_objects_stage_1_3.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage1_3::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(18)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage1_2 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(20), MGC_CELL2PIXEL(22)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage1_4 }
    );

    gate_3_.spawn({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(3)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.set_scene_transition_request(
        { SceneId::Stage1_5 }
    );
    gate_3_.close();

    walker_1_.spawn({MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(20)}, true);
    walker_2_.spawn({MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(12)}, true);
    walker_3_.spawn({MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(22)}, true);

    button_.spawn({MGC_CELL2PIXEL(1), MGC_CELL2PIXEL(3)});

    pushable_block_.spawn({MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(3)});
}


} // namespace app

