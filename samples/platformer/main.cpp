/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "system/system.h"
#include "screen/main/main_screen.hpp"
#include "screen/sub/sub_screen.hpp"

auto main_scn = mgc::main_screen::Screen(MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(0), 15, 12);
auto sub_scn  = mgc::sub_screen::Screen(MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(12), 15, 8);

int main(void) {

    sys_init_hw();

    main_scn.init(mgc::main_screen::SceneId::ID_000);
    sub_scn.init();

    while (true) {
        sys_update_frame_rate();
        sys_gamepad_proc();
        main_scn.update();
        sub_scn.update();
    }
    return 0;
}
