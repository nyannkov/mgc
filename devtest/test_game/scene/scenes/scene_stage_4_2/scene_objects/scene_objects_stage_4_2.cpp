#include "scene_objects_stage_4_2.hpp"
#include "resources/generated/talkscript/talkscript_1.h"

namespace app {

void SceneObjects_Stage4_2::init() {

    gate_1_.spawn({MGC_CELL2PIXEL(88), MGC_CELL2PIXEL(34)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage4_3 }
    );
//    signboard_.spawn({MGC_CELL2PIXEL(87), MGC_CELL2PIXEL(34)});
//    signboard_.set_talkflow_request({
//        &talkscript_1,
//        MGC_TALKSCRIPT_1_PREPARING,
//        nullptr,
//        TalkflowEffectType::Mute
//    });

    gate_2_.spawn({MGC_CELL2PIXEL(21), MGC_CELL2PIXEL(18)});
    gate_2_.set_gate_type(prop::GateType::Type2);
    gate_2_.open();
    gate_2_.set_scene_transition_request(
        { SceneId::Stage4_1, 2 }
    );

    gate_3_.spawn({MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(71)});
    gate_3_.set_gate_type(prop::GateType::Type2);
    gate_3_.open();
    gate_3_.set_scene_transition_request(
        { SceneId::Stage4_1, 3 }
    );

    gate_4_.spawn({MGC_CELL2PIXEL(120), MGC_CELL2PIXEL(47)});
    gate_4_.set_gate_type(prop::GateType::Type2);
    gate_4_.open();
    gate_4_.set_scene_transition_request(
        { SceneId::Stage4_1, 4 }
    );

    portal_1_.spawn({MGC_CELL2PIXEL(47), MGC_CELL2PIXEL(31)-8});

//    walker_1_.spawn({MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(9)}, true);

    //  pair
    teleporters_[0].spawn(
        {MGC_CELL2PIXEL(33), MGC_CELL2PIXEL(39)},
        {MGC_CELL2PIXEL(90), MGC_CELL2PIXEL(18)}
    );
    teleporters_[1].spawn(
        {MGC_CELL2PIXEL(90), MGC_CELL2PIXEL(18)},
        {MGC_CELL2PIXEL(33), MGC_CELL2PIXEL(39)}
    );
    teleporters_[2].spawn(
        {MGC_CELL2PIXEL(123), MGC_CELL2PIXEL(16)},
        {MGC_CELL2PIXEL(152), MGC_CELL2PIXEL(65)}
    );
    teleporters_[3].spawn(
        {MGC_CELL2PIXEL(152), MGC_CELL2PIXEL(65)},
        {MGC_CELL2PIXEL(123), MGC_CELL2PIXEL(16)}
    );
    teleporters_[4].spawn(
        {MGC_CELL2PIXEL(153), MGC_CELL2PIXEL(21)},
        {MGC_CELL2PIXEL(138), MGC_CELL2PIXEL(37)}
    );
    teleporters_[5].spawn(
        {MGC_CELL2PIXEL(138), MGC_CELL2PIXEL(37)},
        {MGC_CELL2PIXEL(153), MGC_CELL2PIXEL(21)}
    );
    teleporters_[6].spawn(
        {MGC_CELL2PIXEL(133), MGC_CELL2PIXEL(44)},
        {MGC_CELL2PIXEL(120), MGC_CELL2PIXEL(26)}
    );
    teleporters_[7].spawn(
        {MGC_CELL2PIXEL(120), MGC_CELL2PIXEL(26)},
        {MGC_CELL2PIXEL(133), MGC_CELL2PIXEL(44)}
    );
    teleporters_[8].spawn(
        {MGC_CELL2PIXEL(66), MGC_CELL2PIXEL(43)},
        {MGC_CELL2PIXEL(60), MGC_CELL2PIXEL(36)}
    );
    teleporters_[9].spawn(
        {MGC_CELL2PIXEL(60), MGC_CELL2PIXEL(36)},
        {MGC_CELL2PIXEL(66), MGC_CELL2PIXEL(43)}
    );
    teleporters_[10].spawn(
        {MGC_CELL2PIXEL(79), MGC_CELL2PIXEL(50)},
        {MGC_CELL2PIXEL(12), MGC_CELL2PIXEL(45)}
    );
    teleporters_[11].spawn(
        {MGC_CELL2PIXEL(12), MGC_CELL2PIXEL(45)},
        {MGC_CELL2PIXEL(79), MGC_CELL2PIXEL(50)}
    );
    teleporters_[12].spawn(
        {MGC_CELL2PIXEL(34), MGC_CELL2PIXEL(31)},
        {MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(18)}
    );
    teleporters_[13].spawn(
        {MGC_CELL2PIXEL(10), MGC_CELL2PIXEL(18)},
        {MGC_CELL2PIXEL(34), MGC_CELL2PIXEL(31)}
    );

    teleporters_[14].spawn(
        {MGC_CELL2PIXEL(125), MGC_CELL2PIXEL(27)},
        {MGC_CELL2PIXEL(123), MGC_CELL2PIXEL(65)}
    );
    teleporters_[15].spawn(
        {MGC_CELL2PIXEL(123), MGC_CELL2PIXEL(65)},
        {MGC_CELL2PIXEL(125), MGC_CELL2PIXEL(27)}
    );
//    teleporters_[16].spawn(
//        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(5)},
//        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(30)}
//    );
//    teleporters_[17].spawn(
//        {MGC_CELL2PIXEL(15), MGC_CELL2PIXEL(14)},
//        {MGC_CELL2PIXEL(23), MGC_CELL2PIXEL(27)}
//    );
//    teleporters_[18].spawn(
//        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(17)},
//        {MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(8)}
//    );
//    teleporters_[19].spawn(
//        {MGC_CELL2PIXEL(17), MGC_CELL2PIXEL(8)},
//        {MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(17)}
//    );
}


} // namespace app

