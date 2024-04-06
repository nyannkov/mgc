cmake_minimum_required(VERSION 3.16)

add_library(display_lcd STATIC display_lcd.c)

target_include_directories(display_lcd PRIVATE
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

if( MGC_DISPLAY_LCD_TYPE STREQUAL "ili9341" )
    target_compile_definitions(display_lcd PRIVATE MGC_DISPLAY_LCD_TYPE=1)
    add_subdirectory(ili9341)
    target_link_libraries(display_lcd display_lcd_ili9341)

else()
    message(FATAL_ERROR "No display-lcd selected.")

endif()

