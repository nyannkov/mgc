cmake_minimum_required(VERSION 3.16)

add_library(system STATIC
    system.c
)

add_subdirectory(resources)
add_subdirectory(${MGC_DEVICES_PATH}/mgc/gamepad/sample gamepad_sample)
set(MGC_DISPLAY_LCD_TYPE ili9341)
add_subdirectory(${MGC_DEVICES_PATH}/mgc/display/lcd display_lcd)

target_include_directories(system PRIVATE
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

target_link_libraries(system
    gamepad_sample
    display_lcd
    sys_resources_gen
)

