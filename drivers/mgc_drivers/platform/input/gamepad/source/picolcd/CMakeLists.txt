add_library(gamepad_picolcd STATIC
    gamepad_picolcd.c
)

target_include_directories(gamepad_picolcd PUBLIC
    ${MGC_SRC_DIR}
    ${MGC_DRIVERS_DIR}/mgc_drivers/platform/input/gamepad/core
    ${MGC_DRIVERS_DIR}/mgc_drivers/platform/input/gamepad/core/cpp
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
)

target_link_libraries(gamepad_picolcd
    pico_stdlib
    hardware_gpio
)

