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

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

extern const unsigned int ST7789_PORT_LCD_RST_PIN;
extern const unsigned int ST7789_PORT_LCD_DC_PIN;
extern const unsigned int ST7789_PORT_LCD_BL_PIN;
extern const unsigned int ST7789_PORT_LCD_CS_PIN;
extern const unsigned int ST7789_PORT_LCD_CLK_PIN;
extern const unsigned int ST7789_PORT_LCD_MOSI_PIN;

void ST7789_PORT__Init(uint32_t spi_clock_rate);
void ST7789_PORT__Delay_ms(UDOUBLE xms);
void ST7789_PORT__Digital_Write(UWORD Pin, UBYTE Value);
void ST7789_PORT__SET_PWM(uint8_t Value);
void ST7789_PORT__GPIO_Mode(UWORD Pin, UWORD Mode);
void ST7789_PORT__SPI_WriteByte(uint8_t Value);
void ST7789_PORT__SPI_Write_nByte(uint8_t pData[], uint32_t Len);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_ST7789_PORT_H*/
