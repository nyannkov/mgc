add_library(mml_psg_port STATIC
    mml_psg_rp2040.c
)

set(MGC_MML_PSG_RP2040_ALARM_NUM       "" CACHE STRING "Override: Alarm number for MML PSG")
set(MGC_MML_PSG_RP2040_ALARM_IRQ_PRI   "" CACHE STRING "Override: Alarm IRQ priority")
set(MGC_MML_PSG_RP2040_PWM_IRQ_PRI     "" CACHE STRING "Override: PWM IRQ priority")
set(MGC_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT "" CACHE STRING "Override: GPIO pin for PSG output")

foreach(OPTION
    MGC_MML_PSG_RP2040_ALARM_NUM
    MGC_MML_PSG_RP2040_ALARM_IRQ_PRI
    MGC_MML_PSG_RP2040_PWM_IRQ_PRI
    MGC_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT
)
    if(NOT "${${OPTION}}" STREQUAL "")
        target_compile_definitions(mgc_drivers_mml_psg PUBLIC ${OPTION}=${${OPTION}})
    endif()
endforeach()

target_link_libraries(mml_psg_port
    mgc_sound_mml_psg
    pico_stdlib
    hardware_pwm
    hardware_irq
)

