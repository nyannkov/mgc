#include "app_common.hpp"
#include "resources/generated/font/k8x12.h"

namespace app {

void load_default_config(TalkflowControllerT& talkflow) {
    talkflow.set_font(k8x12);
    talkflow.set_selectbox_config(
        {
            {170, 48}, // position
            {48, 30}, // size
            "*", // cursor simbol
            {4, 0}, // cursor offset
        }
    );
    talkflow.set_dialoguebox_config(
        {
            {8, 8}, // position
            {216, 72}, // size
            {12, 12, 8, 8}, // padding
            1, // scroll speed
            2, // scroll threshold line
            3, // line spacing
            2, // typing speed
        }
    );
}

bool point_in_box(
    int32_t x, int32_t y,
    int32_t l, int32_t r,
    int32_t t, int32_t b
) {
    return (x >= l && x <= r &&
            y >= t && y <= b);
}

}

