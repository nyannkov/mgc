cmake_minimum_required(VERSION 3.16)

add_library(display_lcd_ili9341 INTERFACE)

add_subdirectory(rp2040)
target_link_libraries(display_lcd_ili9341 INTERFACE display_lcd_ili9341_rp2040)

