/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_ST7789_H
#define MGC_DRIVERS_ST7789_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"


#ifndef MGC_DRIVERS_ST7789_WIDTH
#define MGC_DRIVERS_ST7789_WIDTH             (240)
#endif/*MGC_DRIVERS_ST7789_WIDTH*/

#ifndef MGC_DRIVERS_ST7789_HEIGHT
#define MGC_DRIVERS_ST7789_HEIGHT            (240)
#endif/*MGC_DRIVERS_ST7789_HEIGHT*/


void st7789_init(uint32_t clock_rate);
void st7789_reset(void);
void st7789_display_on(void);
void st7789_display_off(void);
void st7789_inversion_on(void);
void st7789_inversion_off(void);
void st7789_sleep_out(void);
void st7789_sleep_in(void);
bool st7789_transfer_region_blocking_rgb565(uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
bool st7789_transfer_full_region_blocking_rgb565(uint8_t *buffer, size_t len);
bool st7789_transfer_full_region_async_rgb565(uint8_t *buffer, size_t len);
bool st7789_is_busy(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_ST7789_H*/
