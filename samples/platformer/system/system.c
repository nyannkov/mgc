#include <stdio.h>
#include <stdarg.h>
#include <hardware/uart.h>
#include <hardware/gpio.h>
#include <hardware/timer.h>
#include "system.h"
#include "mgc/sound/psg/sound_psg.h"
#include "mgc/display/lcd/display_lcd.h"
#include "mgc/gamepad/sample/gamepad_sample.h"
#include "resources/generates/font/k8x12S.h"

//TODO Remove rp2040 dependencies from this source file.

#define USE_UART            uart0
#define UART_BAUD_RATE      115200
#define UART_TX_PIN         17
#define UART_RX_PIN         16

static uint64_t time_mark[NUM_OF_TIMER_SLOT];
static float frame_rate;

static uint64_t get_time(void) {
    uint32_t lo = timer_hw->timelr;
    uint32_t hi = timer_hw->timehr;
    return ((uint64_t) hi << 32u) | lo;
}

void sys_init_hw(void) {
    uart_init(USE_UART, UART_BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    sys_get_display_driver()->init();
    sys_get_gamepad_driver()->init();
    sys_get_sound_driver()->common_if->init();

    init_sound_lst();
}

void sys_mark_timer_value(enum timer_slot slot_num) {
    if ( slot_num >= NUM_OF_TIMER_SLOT ) {
        MGC_WARN("Invalid timer slot number");
        return;
    }
    time_mark[slot_num] = get_time();
}

uint64_t sys_get_timer_elapsed_value(enum timer_slot slot_num) {
    if ( slot_num >= NUM_OF_TIMER_SLOT ) {
        MGC_WARN("Invalid timer slot number");
        return 0;
    }
    uint64_t time = get_time();
    return (uint64_t)(time - time_mark[slot_num]);
}

void sys_update_frame_rate(void) {
    uint64_t elapsed_value;
    float rate;
    elapsed_value = sys_get_timer_elapsed_value(TIMER_SLOT_FRAME_RATE_TIMER); // usec.
    sys_mark_timer_value(TIMER_SLOT_FRAME_RATE_TIMER);
    if ( elapsed_value != 0 ) {
        rate = (1000000.0/elapsed_value); // fps
    } else {
        rate = 0.0F;
    }
    frame_rate = rate;
}

float sys_get_frame_rate(void) {
    return frame_rate;
}

void sys_printf(const char *format, ...) {
    static char buf[256];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    uart_puts(USE_UART, buf);
}

const mgc_display_if_t *sys_get_display_driver(void) {
    return display_lcd_get_instance();
}

const mgc_gamepad_if_t *sys_get_gamepad_driver(void) {
    return gamepad_sample_get_instance();
}

void sys_gamepad_proc(void) {
    gamepad_sample_proc();
}

const mgc_sound_psg_if_t *sys_get_sound_driver(void) {
    return sound_psg_get_instance();
}

const mgc_font_t *sys_get_default_font(void) {
    return &k8x12S;
}
