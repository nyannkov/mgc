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

static inline uint16_t ST7789_get_width(void) {
    return MGC_DRIVERS_ST7789_WIDTH;
}

static inline uint16_t ST7789_get_height(void) {
    return MGC_DRIVERS_ST7789_HEIGHT;
}

void ST7789_init(uint32_t spi_clock_rate);
void ST7789_reset(void);
void ST7789_transfer_region(uint8_t *buffer, size_t len, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_ST7789_H*/
