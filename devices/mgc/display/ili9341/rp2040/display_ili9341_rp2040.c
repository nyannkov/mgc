/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 *
 * This source code is intended for the LCD module (MSP2807) listed at the
 * following URL:
 *      http://www.lcdwiki.com/2.8inch_SPI_Module_ILI9341_SKU:MSP2807
 * The initialization process for the MSP2807 is created with reference to the
 * sample code on this site.
 *
 */
#include <pico/stdlib.h>
#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include "display_ili9341_rp2040.h"

#ifndef MGC_LCD_SPI
#define MGC_LCD_SPI              spi1
#endif/*MGC_LCD_SPI*/

#ifndef MGC_PIN_LCD_SPI_CLK
#define MGC_PIN_LCD_SPI_CLK      10
#endif/*MGC_PIN_LCD_SPI_CLK*/

#ifndef MGC_PIN_LCD_SPI_TX
#define MGC_PIN_LCD_SPI_TX       11
#endif/*MGC_PIN_LCD_SPI_TX*/

#ifndef MGC_PIN_LCD_LED
#define MGC_PIN_LCD_LED          7
#endif/*MGC_PIN_LCD_LED*/

#ifndef MGC_PIN_LCD_CS
#define MGC_PIN_LCD_CS           9
#endif/*MGC_PIN_LCD_CS*/

#ifndef MGC_PIN_LCD_RS
#define MGC_PIN_LCD_RS           6
#endif/*MGC_PIN_LCD_RS*/

#ifndef MGC_PIN_LCD_RESET
#define MGC_PIN_LCD_RESET        5
#endif/*MGC_PIN_LCD_RESET*/

#ifndef MGC_LCD_SPI_CLK_SPEED
#define MGC_LCD_SPI_CLK_SPEED    40000000 // NOTE: Overclocking (40MHz)
#endif/*MGC_LCD_SPI_CLK_SPEED*/

#define USE_SPI          MGC_LCD_SPI
#define PIN_LED          MGC_PIN_LCD_LED
#define PIN_CS           MGC_PIN_LCD_CS
#define PIN_RS           MGC_PIN_LCD_RS
#define PIN_RESET        MGC_PIN_LCD_RESET
#define PIN_SPI_CLK      MGC_PIN_LCD_SPI_CLK
#define PIN_SPI_TX       MGC_PIN_LCD_SPI_TX

#define ENABLE_LCD_WRITE()          gpio_put(PIN_CS, 0)
#define DISABLE_LCD_WRITE()         gpio_put(PIN_CS, 1)
#define SET_ADDRESS_WRITE_MODE()    gpio_put(PIN_RS, 0)
#define SET_DATA_WRITE_MODE()       gpio_put(PIN_RS, 1)

static void init_spi(void);
static void pin_config(void);
static void ili9341_init(void);

static int display_init(void);
static void display_reset(void);
static int display_draw_rect(uint8_t *buf, size_t len, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

static inline void ili9341_write_byte(uint8_t dat) {
    spi_write_blocking(USE_SPI, &dat, 1);
}

static inline void ili9341_write_byte_array(const uint8_t *dat_array, size_t dat_count) {
    spi_write_blocking(USE_SPI, dat_array, dat_count);
}

static inline void ili9341_write_command(uint8_t addr, const uint8_t *dat_array, size_t dat_count) {
    SET_ADDRESS_WRITE_MODE();
    spi_write_blocking(USE_SPI, &addr, 1);

    SET_DATA_WRITE_MODE();
    spi_write_blocking(USE_SPI, dat_array, dat_count);
}

static int display_init(void) {

    pin_config();

    init_spi();

    display_reset();

    ili9341_init();

    return 0;
}

static void display_reset(void) {
    gpio_put(PIN_RESET, 1);
    sleep_ms(5);
    gpio_put(PIN_RESET, 0);
    sleep_ms(15);
    gpio_put(PIN_RESET, 1);
    sleep_ms(15);
}

static int display_draw_rect(uint8_t *buf, size_t len, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {

    uint8_t lcd_data[4];
    if ( buf == NULL ) {
        return -1;
    }

    ENABLE_LCD_WRITE();
    {
        // 8.2.20. Column Address Set (2Ah)
        lcd_data[0] = (x0>>8)&0xFF;
        lcd_data[1] = (x0>>0)&0xFF;
        lcd_data[2] = (x1>>8)&0xFF;
        lcd_data[3] = (x1>>0)&0xFF;
        ili9341_write_command(0x2B, lcd_data, 4);

        // 8.2.21. Page Address Set (2Bh)
        lcd_data[0] = (y0>>8)&0xFF;
        lcd_data[1] = (y0>>0)&0xFF;
        lcd_data[2] = (y1>>8)&0xFF;
        lcd_data[3] = (y1>>0)&0xFF;
        ili9341_write_command(0x2A, lcd_data, 4);

        // 8.2.22. Memory Write (2Ch)
        ili9341_write_command(0x2C, buf, len);
    }
    DISABLE_LCD_WRITE();

    return 0;
}

static void ili9341_init(void) {

    ENABLE_LCD_WRITE();
    {
        // 8.4.1 Power control A (CBh)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xCB);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x39);
        ili9341_write_byte(0x2C);
        ili9341_write_byte(0x00);
        ili9341_write_byte(0x34);
        ili9341_write_byte(0x02);

        // 8.4.2 Power control B (CFh)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xCF);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x00);
        ili9341_write_byte(0X81);
        ili9341_write_byte(0X30);

        // 8.4.3 Driver timing control A (E8h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xE8);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x85);
        ili9341_write_byte(0x00);
        ili9341_write_byte(0x78);

        // 8.4.4 Driver timing control B (EAh)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xEA);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x00);
        ili9341_write_byte(0x00);

        // 8.4.5 Power on sequence control (EDh)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xED);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x64);
        ili9341_write_byte(0x03);
        ili9341_write_byte(0X12);
        ili9341_write_byte(0X81);

        // 8.4.7 Pump ratio control (F7h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xF7);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x20);

        // 8.3.16. Power Control 1 (C0h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xC0);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x23);

        // 8.3.17. Power Control 2 (C1h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xC1);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x10);

        // 8.3.18. VCOM Control 1(C5h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xC5);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x3e);
        ili9341_write_byte(0x28);

        // 8.3.19. VCOM Control 2(C7h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xC7);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x86);

        // 8.2.29. Memory Access Control (36h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0x36);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x78);

        // 8.2.33. COLMOD: Pixel Format Set (3Ah)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0x3A);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x55);

        // 8.3.2. Frame Rate Control (In Normal Mode/Full Colors) (B1h)
        // 7.3. VSYNC Interface
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xB1);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x00);
        ili9341_write_byte(0x10);

        // 8.3.7. Display Function Control (B6h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0xB6);
        SET_DATA_WRITE_MODE();
        ili9341_write_byte(0x08);
        ili9341_write_byte(0x82);
        ili9341_write_byte(0x27);

        // 8.2.12. Sleep Out (11h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0x11);
        sleep_ms(120);

        // 8.2.19. Display ON (29h)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0x29);

        // 8.2.22. Memory Write (2Ch)
        SET_ADDRESS_WRITE_MODE();
        ili9341_write_byte(0x2c);
    }
    DISABLE_LCD_WRITE();
}


const mgc_display_if_t  display_driver_ili9341_rp2040 = {
    .init = display_init,
    .reset = display_reset,
    .draw_rect = display_draw_rect
};

static void init_spi(void) {
    spi_init(USE_SPI, MGC_LCD_SPI_CLK_SPEED);
    gpio_set_function(PIN_SPI_CLK, GPIO_FUNC_SPI); // SCK
    gpio_set_function(PIN_SPI_TX,  GPIO_FUNC_SPI); // TX
//    gpio_set_function(PIN_SPI_RX,  GPIO_FUNC_SPI); // RX
    spi_set_format(USE_SPI, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

static void pin_config(void) {
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_put(PIN_LED, 1);

    gpio_init(PIN_RS);
    gpio_set_dir(PIN_RS, GPIO_OUT);
    gpio_put(PIN_RS, 1);

    gpio_init(PIN_RESET);
    gpio_set_dir(PIN_RESET, GPIO_OUT);
    gpio_put(PIN_RESET, 1);

    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
}
