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

#ifndef MGC_DRIVERS_ST7789_PORT__LCD_RST_PIN
#define MGC_DRIVERS_ST7789_PORT__LCD_RST_PIN         12
#endif/*MGC_DRIVERS_ST7789_PORT__LCD_RST_PIN*/

#ifndef MGC_DRIVERS_ST7789_PORT__LCD_DC_PIN
#define MGC_DRIVERS_ST7789_PORT__LCD_DC_PIN           8
#endif/*MGC_DRIVERS_ST7789_PORT__LCD_DC_PIN*/

#ifndef MGC_DRIVERS_ST7789_PORT__LCD_BL_PIN
#define MGC_DRIVERS_ST7789_PORT__LCD_BL_PIN          13
#endif/*MGC_DRIVERS_ST7789_PORT__LCD_BL_PIN*/

#ifndef MGC_DRIVERS_ST7789_PORT__LCD_CS_PIN 
#define MGC_DRIVERS_ST7789_PORT__LCD_CS_PIN           9
#endif/*MGC_DRIVERS_ST7789_PORT__LCD_CS_PIN*/

#ifndef MGC_DRIVERS_ST7789_PORT__LCD_CLK_PIN
#define MGC_DRIVERS_ST7789_PORT__LCD_CLK_PIN         10
#endif/*MGC_DRIVERS_ST7789_PORT__LCD_CLK_PIN*/

#ifndef MGC_DRIVERS_ST7789_PORT__LCD_MOSI_PIN
#define MGC_DRIVERS_ST7789_PORT__LCD_MOSI_PIN        11
#endif/*MGC_DRIVERS_ST7789_PORT__LCD_MOSI_PIN*/

enum st7789_transfer_state {
    ST7789_TRANSFER_STOP,
    ST7789_TRANSFER_RUNNING,
    ST7789_TRANSFER_END,
    ST7789_TRANSFER_ERROR,
};


void st7789_port__gpio_output_init(uint32_t pin, bool value);
void st7789_port__gpio_write(uint32_t pin, bool value); 
void st7789_port__spi_init(uint32_t clock_rate);
bool st7789_port__spi_transfer_blocking(const uint8_t *buffer, size_t len);
bool st7789_port__spi_transfer_async(const uint8_t *buffer, size_t len);
enum st7789_transfer_state st7789_port__spi_get_transfer_state(void);

void st7789_port__sleep_ms(uint32_t ms);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_ST7789_PORT_H*/
