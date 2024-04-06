cmake_minimum_required(VERSION 3.16)

add_library(gamepad_sample_rp2040 STATIC
    gamepad_sample_rp2040.c
)

target_include_directories(gamepad_sample_rp2040 PRIVATE
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

target_link_libraries(gamepad_sample_rp2040
    hardware_gpio
    hardware_adc
)

