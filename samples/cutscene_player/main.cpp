/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "system/system.h"
#include "screen/screen.hpp"

#define SCREEN_UPDATE_RATE  76923 // 76.923 ms. -> 13 Hz

int main(void) {

    auto screen = mgc::screen::Screen(MGC_CELL2PIXEL(0), MGC_CELL2PIXEL(0), 15, 12);
    const auto gamepad = sys_get_gamepad_driver();

    sys_init_hw();

    screen.init(mgc::screen::SceneId::ID_000);

    sys_mark_timer_value(TIMER_SLOT_UPDATE_TIMER);

    while (true) {
        if ( sys_get_timer_elapsed_value(TIMER_SLOT_UPDATE_TIMER) >= SCREEN_UPDATE_RATE ) {
            sys_mark_timer_value(TIMER_SLOT_UPDATE_TIMER);
            sys_update_frame_rate();
            gamepad->proc();
            screen.update();
        }
    }
    return 0;
}
