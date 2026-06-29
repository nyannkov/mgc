#include "scene_objects_stage_2_5.hpp"
#include "resources/generated/talkscript/talkscript_1.h"
#include "resources/generated/font/k8x12.h"

namespace app {

void SceneObjects_Stage2_5::init() {

    int16_t ofs_x = 0;

    gate_1_.spawn({MGC_CELL2PIXEL(14), MGC_CELL2PIXEL(14)});
    gate_1_.set_gate_type(prop::GateType::Type2);
    gate_1_.open();
    gate_1_.set_scene_transition_request(
        { SceneId::Stage2_3 }
    );

    label_counter_.set_position({MGC_CELL2PIXEL(7), MGC_CELL2PIXEL(11)});
    label_counter_.set_size({36, 36});
    label_counter_.set_font(k8x12);
    label_counter_.set_fontsize2x(true);
    label_counter_.set_font_fore_color(MGC_COLOR(0,1,0));
    label_counter_.set_enabled_back_color(false);
    label_counter_.set_text("");
    label_counter_.set_parallax_factor({1, 1});

    for ( size_t i = 0; i < buttons_.size(); ++i ) {
        buttons_[i].set_pushed_state(true);
    }
}


} // namespace app

