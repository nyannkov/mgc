/*****************************************************************************
* | File      	:   LCD_1in3.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V1.0
* | Date        :   2020-05-20
* | Info        :   Basic version
*
******************************************************************************/

/*****************************************************************************
* This software is based on and modified from the sample code provided by Waveshare.
*
* Reference:
* https://www.waveshare.com/wiki/Pico-LCD-1.3
******************************************************************************/

/*
 * Sample Implementation Notice
 *
 * This file is a sample.
 * Please rewrite or replace it according to your project's requirements.
 */

#include "mgc_drivers/platform/display/ST7789/ports/ST7789_port.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"

static uint slice_num;

#ifndef MGC_DRIVERS_ST7789_RP2040_SPI_PORT
#define MGC_DRIVERS_ST7789_RP2040_SPI_PORT    spi1
#endif/*MGC_DRIVERS_ST7789_RP2040_SPI_PORT*/

#ifndef MGC_DRIVERS_ST7789_RP2040_SCAN_DIR
#define MGC_DRIVERS_ST7789_RP2040_SCAN_DIR    HORIZONTAL
#endif/*MGC_DRIVERS_ST7789_RP2040_SCAN_DIR*/

/**
 * GPIO config
**/
#ifndef MGC_DRIVERS_ST7789_RP2040_LCD_RST_PIN
#define MGC_DRIVERS_ST7789_RP2040_LCD_RST_PIN  12
#endif/*MGC_DRIVERS_ST7789_RP2040_LCD_RST_PIN*/

#ifndef MGC_DRIVERS_ST7789_RP2040_LCD_DC_PIN
#define MGC_DRIVERS_ST7789_RP2040_LCD_DC_PIN   8
#endif/*MGC_DRIVERS_ST7789_RP2040_LCD_DC_PIN*/

#ifndef MGC_DRIVERS_ST7789_RP2040_LCD_BL_PIN
#define MGC_DRIVERS_ST7789_RP2040_LCD_BL_PIN   13
#endif/*MGC_DRIVERS_ST7789_RP2040_LCD_BL_PIN*/

#ifndef MGC_DRIVERS_ST7789_RP2040_LCD_CS_PIN 
#define MGC_DRIVERS_ST7789_RP2040_LCD_CS_PIN   9
#endif/*MGC_DRIVERS_ST7789_RP2040_LCD_CS_PIN*/

#ifndef MGC_DRIVERS_ST7789_RP2040_LCD_CLK_PIN
#define MGC_DRIVERS_ST7789_RP2040_LCD_CLK_PIN  10
#endif/*MGC_DRIVERS_ST7789_RP2040_LCD_CLK_PIN*/

#ifndef MGC_DRIVERS_ST7789_RP2040_LCD_MOSI_PIN
#define MGC_DRIVERS_ST7789_RP2040_LCD_MOSI_PIN 11
#endif/*MGC_DRIVERS_ST7789_RP2040_LCD_MOSI_PIN*/

#define LCD_SPI_PORT  MGC_DRIVERS_ST7789_RP2040_SPI_PORT

const unsigned int ST7789_PORT_LCD_RST_PIN =  MGC_DRIVERS_ST7789_RP2040_LCD_RST_PIN;
const unsigned int ST7789_PORT_LCD_DC_PIN =   MGC_DRIVERS_ST7789_RP2040_LCD_DC_PIN;
const unsigned int ST7789_PORT_LCD_BL_PIN =   MGC_DRIVERS_ST7789_RP2040_LCD_BL_PIN;
const unsigned int ST7789_PORT_LCD_CS_PIN =   MGC_DRIVERS_ST7789_RP2040_LCD_CS_PIN;
const unsigned int ST7789_PORT_LCD_CLK_PIN =  MGC_DRIVERS_ST7789_RP2040_LCD_CLK_PIN;
const unsigned int ST7789_PORT_LCD_MOSI_PIN = MGC_DRIVERS_ST7789_RP2040_LCD_MOSI_PIN;

#define LCD_SPI_DEFAULT_CLOCK_RATE      (10*1000*1000)

void ST7789_PORT__Init(uint32_t spi_clock_rate) {

    if ( spi_clock_rate == 0 ) {
        spi_clock_rate = LCD_SPI_DEFAULT_CLOCK_RATE;
    }

    // SPI Config
    spi_init(LCD_SPI_PORT, spi_clock_rate);
    gpio_set_function(ST7789_PORT_LCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(ST7789_PORT_LCD_MOSI_PIN, GPIO_FUNC_SPI);
    
    // GPIO Config
    ST7789_PORT__GPIO_Mode(ST7789_PORT_LCD_RST_PIN, 1);
    ST7789_PORT__GPIO_Mode(ST7789_PORT_LCD_DC_PIN, 1);
    ST7789_PORT__GPIO_Mode(ST7789_PORT_LCD_CS_PIN, 1);
    ST7789_PORT__GPIO_Mode(ST7789_PORT_LCD_BL_PIN, 1);
    ST7789_PORT__Digital_Write(ST7789_PORT_LCD_CS_PIN, 1);
    ST7789_PORT__Digital_Write(ST7789_PORT_LCD_DC_PIN, 0);
    ST7789_PORT__Digital_Write(ST7789_PORT_LCD_BL_PIN, 1);
    
    // PWM Config
    gpio_set_function(ST7789_PORT_LCD_BL_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(ST7789_PORT_LCD_BL_PIN);
    pwm_set_wrap(slice_num, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 1);
    pwm_set_clkdiv(slice_num,50);
    pwm_set_enabled(slice_num, true);
}

/**
 * delay x ms
**/
void ST7789_PORT__Delay_ms(UDOUBLE xms) {
    uint32_t start = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) - start < xms);
}

/**
 * GPIO read and write
**/
void ST7789_PORT__Digital_Write(UWORD Pin, UBYTE Value) {
    gpio_put(Pin, Value);
}

void ST7789_PORT__SET_PWM(uint8_t Value) {
    if(Value<0 || Value >100){
        //printf("ST7789_PORT__SET_PWM Error \r\n");
    }else {
        pwm_set_chan_level(slice_num, PWM_CHAN_B, Value);
    }
}

/**
 * GPIO Mode
**/
void ST7789_PORT__GPIO_Mode(UWORD Pin, UWORD Mode) {
    gpio_init(Pin);
    if(Mode == 0 || Mode == GPIO_IN) {
        gpio_set_dir(Pin, GPIO_IN);
    } else {
        gpio_set_dir(Pin, GPIO_OUT);
    }
}

/**
 * SPI
**/
void ST7789_PORT__SPI_WriteByte(uint8_t Value) {
    spi_write_blocking(LCD_SPI_PORT, &Value, 1);
}

void ST7789_PORT__SPI_Write_nByte(uint8_t pData[], uint32_t Len) {
    spi_write_blocking(LCD_SPI_PORT, pData, Len);
}

