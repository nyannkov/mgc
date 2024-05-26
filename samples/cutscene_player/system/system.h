#ifndef MGC_SYSTEM_H
#define MGC_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/sound/psg/common/sound_psg_if.h"
#include "mgc/display/lcd/common/display_lcd_if.h"
#include "mgc/gamepad/interface/gamepad_if.h"
#include "mgc/font/font.h"

enum timer_slot {
    TIMER_SLOT_FRAME_RATE_TIMER = 0,
    TIMER_SLOT_UPDATE_TIMER,
    // Add slots as needed.
    NUM_OF_TIMER_SLOT
};
void sys_init_hw(void);

void sys_mark_timer_value(enum timer_slot slot_num);
uint64_t sys_get_timer_elapsed_value(enum timer_slot slot_num);
void sys_update_frame_rate(void);
float sys_get_frame_rate(void);

void sys_printf(const char *format, ...);
const mgc_display_lcd_if_t *sys_get_display_driver(void);
const mgc_sound_psg_if_t *sys_get_sound_driver(void);
const mgc_gamepad_if_t *sys_get_gamepad_driver(void);
void sys_gamepad_proc(void);
const mgc_font_t *sys_get_default_font(void);

uint32_t sys_save_and_disable_interrupts(void);
void sys_restore_interrupts(uint32_t status);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_SYSTEM_H*/
