cmake_minimum_required(VERSION 3.16)

add_library(display_lcd_ili9341_rp2040 STATIC
    display_lcd_ili9341_rp2040.c
)

target_include_directories(display_lcd_ili9341_rp2040 PRIVATE
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

target_link_libraries(display_lcd_ili9341_rp2040
    pico_stdlib
    hardware_gpio
    hardware_spi
)

