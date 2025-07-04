/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */

/*
 * Sample Implementation Notice
 *
 * This file is a sample.
 * Please rewrite or replace it according to your project's requirements.
 */

#include <pico/time.h>

uint32_t free_running_timer_port__get_now_ms(void) {
    return to_ms_since_boot(get_absolute_time());
}

