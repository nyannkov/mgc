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

#include <pico/stdlib.h>
#include <hardware/spi.h>
#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/sync.h>
#include "mgc_drivers/platform/display/st7789/ports/st7789_port.h"

#ifndef MGC_DRIVERS_ST7789_RP2040_SPI_PORT
#define MGC_DRIVERS_ST7789_RP2040_SPI_PORT              spi1
#endif/*MGC_DRIVERS_ST7789_RP2040_SPI_PORT*/

#ifndef MGC_DRIVERS_ST7789_RP2040_SPI_CLOCK_RATE
#define MGC_DRIVERS_ST7789_RP2040_SPI_CLOCK_RATE   (10*1000*1000)
#endif/*MGC_DRIVERS_ST7789_RP2040_SPI_CLOCK_RATE*/

#ifndef MGC_DRIVERS_ST7789_RP2040_DMA_IRQ_PRI
#define MGC_DRIVERS_ST7789_RP2040_DMA_IRQ_PRI           0xC0
#endif/*MGC_DRIVERS_ST7789_RP2040_DMA_IRQ_PRI*/

#ifndef MGC_DRIVERS_ST7789_RP2040_PIN_RST
#define MGC_DRIVERS_ST7789_RP2040_PIN_RST           12
#endif/*MGC_DRIVERS_ST7789_RP2040_PIN_RST*/

#ifndef MGC_DRIVERS_ST7789_RP2040_PIN_DC
#define MGC_DRIVERS_ST7789_RP2040_PIN_DC            8
#endif/*MGC_DRIVERS_ST7789_RP2040_PIN_DC*/

#ifndef MGC_DRIVERS_ST7789_RP2040_PIN_BL
#define MGC_DRIVERS_ST7789_RP2040_PIN_BL            13
#endif/*MGC_DRIVERS_ST7789_RP2040_PIN_BL*/

#ifndef MGC_DRIVERS_ST7789_RP2040_PIN_CS 
#define MGC_DRIVERS_ST7789_RP2040_PIN_CS            9
#endif/*MGC_DRIVERS_ST7789_RP2040_PIN_CS*/

#ifndef MGC_DRIVERS_ST7789_RP2040_PIN_CLK
#define MGC_DRIVERS_ST7789_RP2040_PIN_CLK           10
#endif/*MGC_DRIVERS_ST7789_RP2040_PIN_CLK*/

#ifndef MGC_DRIVERS_ST7789_RP2040_PIN_MOSI
#define MGC_DRIVERS_ST7789_RP2040_PIN_MOSI          11
#endif/*MGC_DRIVERS_ST7789_RP2040_PIN_MOSI*/



#define LCD_SPI_PORT                MGC_DRIVERS_ST7789_RP2040_SPI_PORT
#define LCD_SPI_CLOCK_RATE          MGC_DRIVERS_ST7789_RP2040_SPI_CLOCK_RATE
#define LCD_DMA_IRQ_PRI             MGC_DRIVERS_ST7789_RP2040_DMA_IRQ_PRI             

#define LCD_PIN_RST                 MGC_DRIVERS_ST7789_RP2040_PIN_RST
#define LCD_PIN_DC                  MGC_DRIVERS_ST7789_RP2040_PIN_DC
#define LCD_PIN_BL                  MGC_DRIVERS_ST7789_RP2040_PIN_BL
#define LCD_PIN_CS                  MGC_DRIVERS_ST7789_RP2040_PIN_CS
#define LCD_PIN_CLK                 MGC_DRIVERS_ST7789_RP2040_PIN_CLK
#define LCD_PIN_MOSI                MGC_DRIVERS_ST7789_RP2040_PIN_MOSI


static inline
int to_pin_num(enum st7789_port__pin pin_name) {
    switch (pin_name) {
    case ST7789_PORT__RST: return LCD_PIN_RST;
    case ST7789_PORT__DC:  return LCD_PIN_DC;
    case ST7789_PORT__BL:  return LCD_PIN_BL;
    default:               return -1;
    }
}

static int dma_chan; 
static void *context;
static void (*on_transfer_async_completed)(void *context);

static void dma_handler(void) {

    while ( spi_is_busy(LCD_SPI_PORT) ) { }
    gpio_put(LCD_PIN_CS, true);

    if ( on_transfer_async_completed ) {
        on_transfer_async_completed(context);
    }

    dma_hw->ints0 = 1u << dma_chan;
}

MGC_WEAK void st7789_port__spi_init(uint32_t clock_rate) {

    if ( clock_rate == 0 ) {
        clock_rate = LCD_SPI_CLOCK_RATE;
    }

    spi_init(LCD_SPI_PORT, clock_rate);
    gpio_set_function(LCD_PIN_CLK, GPIO_FUNC_SPI);
    gpio_set_function(LCD_PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(LCD_PIN_CS);
    gpio_set_dir(LCD_PIN_CS, GPIO_OUT);
    gpio_put(LCD_PIN_CS, true);

    dma_chan = dma_claim_unused_channel(true);
    dma_channel_set_irq0_enabled(dma_chan, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_priority(DMA_IRQ_0, LCD_DMA_IRQ_PRI);
    irq_set_enabled(DMA_IRQ_0, true);
}

void st7789_port__gpio_output_init(enum st7789_port__pin  pin_name, bool value) {
    int pin = to_pin_num(pin_name);
    if ( pin < 0 ) {
        return;
    }
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, value);
}

void st7789_port__gpio_write(enum st7789_port__pin pin_name, bool value) {
    int pin = to_pin_num(pin_name);
    if ( pin < 0 ) {
        return;
    }
    gpio_put(pin, value);
}

bool st7789_port__spi_transfer_blocking(const uint8_t *buffer, size_t len, bool cs_hold) {

    dma_channel_wait_for_finish_blocking(dma_chan);

    gpio_put(LCD_PIN_CS, false);

    spi_write_blocking(LCD_SPI_PORT, buffer, len);

    if ( !cs_hold )
        gpio_put(LCD_PIN_CS, true);

    return true;
}


bool st7789_port__spi_transfer_async(const uint8_t *buffer, size_t len) {

    if (dma_channel_is_busy(dma_chan)) {
        return false;
    }

    dma_channel_config config = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);
    channel_config_set_dreq(&config, spi_get_dreq(LCD_SPI_PORT, true));
    channel_config_set_read_increment(&config, true);
    channel_config_set_write_increment(&config, false);

    gpio_put(LCD_PIN_CS, false);
    
    dma_channel_configure(
        dma_chan,
        &config,
        &spi_get_hw(LCD_SPI_PORT)->dr,
        buffer,
        len,
        true
    );

    return true;
}

bool st7789_port__is_busy(void) {
    return dma_channel_is_busy(dma_chan);
}

void st7789_port__sleep_ms(uint32_t ms) {
    sleep_ms(ms);
}

void st7789_port__wait_for_interrupt(void) {
    __wfi();
}

void st7789_port__set_on_transfer_async_completed(void (*cb)(void* context), void* ctx) {
    context = ctx;
    on_transfer_async_completed = cb;
}

