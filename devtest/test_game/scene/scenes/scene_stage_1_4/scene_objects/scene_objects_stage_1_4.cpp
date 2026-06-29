#include "scene_objects_stage_1_4.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage1_4::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(28), MGC_CELL2PIXEL(30)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage1_3 }
    );

    portal_1_.spawn({MGC_CELL2PIXEL(20), MGC_CELL2PIXEL(8)-8});

    walker_1_.spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(9)}, true);

    //  pair
    teleporters_[0].spawn(
        {MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(27)},
        {MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(27)}
    );
    teleporters_[1].spawn(
        {MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(27)},
        {MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(27)}
    );
    teleporters_[2].spawn(
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(26)},
        {MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(27)}
    );
    teleporters_[3].spawn(
        {MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(27)},
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(26)}
    );
    teleporters_[4].spawn(
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(13)},
        {MGC_CELL2PIXEL(7), MGC_CELL2PIXEL(7)}
    );
    teleporters_[5].spawn(
        {MGC_CELL2PIXEL(7), MGC_CELL2PIXEL(7)},
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(13)}
    );
    teleporters_[6].spawn(
        {MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(5)},
        {MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(2)}
    );
    teleporters_[7].spawn(
        {MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(2)},
        {MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(5)}
    );
    teleporters_[8].spawn(
        {MGC_CELL2PIXEL(29), MGC_CELL2PIXEL(2)},
        {MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(16)}
    );
    teleporters_[9].spawn(
        {MGC_CELL2PIXEL(30), MGC_CELL2PIXEL(16)},
        {MGC_CELL2PIXEL(29), MGC_CELL2PIXEL(2)}
    );
    teleporters_[10].spawn(
        {MGC_CELL2PIXEL(25), MGC_CELL2PIXEL(16)},
        {MGC_CELL2PIXEL(24), MGC_CELL2PIXEL(20)}
    );
    teleporters_[11].spawn(
        {MGC_CELL2PIXEL(24), MGC_CELL2PIXEL(20)},
        {MGC_CELL2PIXEL(25), MGC_CELL2PIXEL(16)}
    );
    teleporters_[12].spawn(
        {MGC_CELL2PIXEL(20), MGC_CELL2PIXEL(12)},
        {MGC_CELL2PIXEL(20), MGC_CELL2PIXEL(17)}
    );
    teleporters_[13].spawn(
        {MGC_CELL2PIXEL(20), MGC_CELL2PIXEL(17)},
        {MGC_CELL2PIXEL(20), MGC_CELL2PIXEL(12)}
    );

    // 
    teleporters_[14].spawn(
        {MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(27)},
        {MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(16)}
    );
    teleporters_[15].spawn(
        {MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(16)},
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(5)}
    );
    teleporters_[16].spawn(
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(5)},
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(30)}
    );
    teleporters_[17].spawn(
        {MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(14)},
        {MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(27)}
    );
    teleporters_[18].spawn(
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(17)},
        {MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(8)}
    );
    teleporters_[19].spawn(
        {MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(8)},
        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(17)}
    );
}


} // namespace app

