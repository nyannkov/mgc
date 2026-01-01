add_library(gamepad_picolcd STATIC
    gamepad_picolcd.c
)

target_include_directories(gamepad_picolcd PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
)

target_link_libraries(gamepad_picolcd
    mgc_input_gamepad
    pico_stdlib
    hardware_gpio
)

