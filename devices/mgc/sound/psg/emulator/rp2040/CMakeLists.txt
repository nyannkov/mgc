cmake_minimum_required(VERSION 3.16)

add_library(sound_psg_emulator_rp2040 STATIC
    sound_psg_emulator_rp2040.cpp
)

add_subdirectory(${MGC_DEVICES_PATH}/mgc/sound/psg/common/Psgino Psgino)
add_subdirectory(${MGC_DEVICES_PATH}/mgc/sound/psg/emulator/emu2149 emu2149)

target_include_directories(sound_psg_emulator_rp2040 PRIVATE
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

target_link_libraries(sound_psg_emulator_rp2040
    pico_stdlib
    hardware_pwm
    hardware_irq
    hardware_gpio
    emu2149
    Psgino
)

