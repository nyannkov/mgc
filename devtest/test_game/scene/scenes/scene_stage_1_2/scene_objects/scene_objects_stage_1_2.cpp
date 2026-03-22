#include "scene_objects_stage_1_2.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage1_2::init() {

    skyfish_1_.spawn({MGC_CELL2PIXEL(14), MGC_CELL2PIXEL(24)}, true);
    skyfish_2_.spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(10)}, true);
    skyfish_3_.spawn({MGC_CELL2PIXEL(8), MGC_CELL2PIXEL(4)}, true);
    
//    elixir_.spawn({MGC_CELL2PIXEL(20), MGC_CELL2PIXEL(25)+8});
    potion_.spawn({MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(10)+8});

    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(30)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage1_1 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(5)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage1_3 }
    );
}


} // namespace app

