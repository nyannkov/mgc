/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_ST7789_PORT_H
#define MGC_DRIVERS_ST7789_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mgc/common/common.h"


enum st7789_port__pin {
    ST7789_PORT__RST,
    ST7789_PORT__DC,
    ST7789_PORT__BL
};

void st7789_port__gpio_output_init(enum st7789_port__pin pin_name, bool value);
void st7789_port__gpio_write(enum st7789_port__pin pin_name, bool value); 
void st7789_port__spi_init(uint32_t clock_rate);
bool st7789_port__spi_transfer_blocking(const uint8_t *buffer, size_t len, bool cs_hold);
bool st7789_port__spi_transfer_async(const uint8_t *buffer, size_t len);

bool st7789_port__is_busy(void);
void st7789_port__sleep_ms(uint32_t ms);
void st7789_port__wait_for_interrupt(void);
void st7789_port__set_on_transfer_async_completed(void (*cb)(void* context), void* ctx);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_ST7789_PORT_H*/
