add_library(mml_psg_port STATIC
    mml_psg_rp2040.c
)

target_link_libraries(mml_psg_port
    mgc_sound_mml_psg
    pico_stdlib
    hardware_pwm
    hardware_irq
)

