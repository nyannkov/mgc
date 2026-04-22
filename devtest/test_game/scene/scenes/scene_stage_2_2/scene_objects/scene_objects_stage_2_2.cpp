#include "scene_objects_stage_2_2.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage2_2::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(72)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage2_1 }
    );

    gate_2_.spawn({MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(2)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage2_3 }
    );

    gate_3_.spawn({MGC_CELL2PIXEL(29), MGC_CELL2PIXEL(2)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.open();
    gate_3_.set_scene_transition_request(
        { SceneId::Stage2_4 }
    );

    floaty_fish_1_.spawn({MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(73)});
    floaty_fish_2_.spawn({MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(54)});
    floaty_fish_3_.spawn({MGC_CELL2PIXEL(7), MGC_CELL2PIXEL(33)});
    floaty_fish_4_.spawn({MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(33)});
    floaty_fish_5_.spawn({MGC_CELL2PIXEL(9), MGC_CELL2PIXEL(12)});
    floaty_fish_6_.spawn({MGC_CELL2PIXEL(22), MGC_CELL2PIXEL(12)});
    skyfish_1_.spawn({MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(24)}, true);
}


} // namespace app

