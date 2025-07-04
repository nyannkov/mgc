/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#include "free_running_timer.h"
#include "ports/free_running_timer_port.h"


uint32_t free_running_timer_get_now_ms(void) {
    return free_running_timer_port__get_now_ms();
}
