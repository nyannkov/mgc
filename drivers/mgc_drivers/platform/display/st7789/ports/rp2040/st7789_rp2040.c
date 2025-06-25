#include <pico/stdlib.h>
#include <hardware/spi.h>
#include "mgc_drivers/platform/display/st7789/ports/st7789_port.h"

#ifndef MGC_DRIVERS_ST7789_RP2040_SPI_PORT
#define MGC_DRIVERS_ST7789_RP2040_SPI_PORT    spi1
#endif/*MGC_DRIVERS_ST7789_RP2040_SPI_PORT*/

#define LCD_SPI_PORT        MGC_DRIVERS_ST7789_RP2040_SPI_PORT

#define LCD_RST_PIN         MGC_DRIVERS_ST7789_PORT__LCD_RST_PIN
#define LCD_DC_PIN          MGC_DRIVERS_ST7789_PORT__LCD_DC_PIN
#define LCD_BL_PIN          MGC_DRIVERS_ST7789_PORT__LCD_BL_PIN
#define LCD_CS_PIN          MGC_DRIVERS_ST7789_PORT__LCD_CS_PIN
#define LCD_CLK_PIN         MGC_DRIVERS_ST7789_PORT__LCD_CLK_PIN
#define LCD_MOSI_PIN        MGC_DRIVERS_ST7789_PORT__LCD_MOSI_PIN

#define LCD_SPI_DEFAULT_CLOCK_RATE      (10*1000*1000)


void st7789_port__spi_init(uint32_t clock_rate) {
    if ( clock_rate == 0 ) {
        clock_rate = LCD_SPI_DEFAULT_CLOCK_RATE;
    }

    spi_init(LCD_SPI_PORT, clock_rate);
    gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);
}

void st7789_port__gpio_output_init(uint32_t pin, bool value) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, value);
}

void st7789_port__gpio_write(uint32_t pin, bool value) {
    gpio_put(pin, value);
}

bool st7789_port__spi_transfer_blocking(const uint8_t *buffer, size_t len) {
    spi_write_blocking(LCD_SPI_PORT, buffer, len);
    return true;
}

bool st7789_port__spi_transfer_async(const uint8_t *buffer, size_t len) {
    return false;
}

enum st7789_transfer_state st7789_port__spi_get_transfer_state(void) {
    return ST7789_TRANSFER_STOP;
}

void st7789_port__sleep_ms(uint32_t ms) {
    sleep_ms(ms);
}
