set(MML_PSG_SRC
    "mml_psg.cpp"
    "${MGC_EXTERNAL_DIR}/sound/emu2149/emu2149.c" # Specify source files directly to bypass the -O3 optimization in emu2149's CMakeLists.txt
)

if(MGC_USE_RP2040)
    list(APPEND MML_PSG_SRC
        "ports/rp2040/mml_psg_rp2040.c"
    )
endif()

add_library(mgc_sound_mml_psg STATIC ${MML_PSG_SRC})

set(MGC_MML_PSG_DEFAULT_LPF_ALPHA "" CACHE STRING "Override: LPF alpha (fixed-point, use F_TO_Q format)")
set(MGC_MML_PSG_LPF_SHIFT_BIT     "" CACHE STRING "Input attenuation factor before LPF (right shift bits)")
set(MGC_MML_PSG_EMULATOR_CLOCK    "" CACHE STRING "Override: PSG emulator clock (Hz)")
set(MGC_MML_PSG_EMULATOR_RATE     "" CACHE STRING "Override: PSG emulator output rate (Hz)")
set(MGC_MML_PSG_MML_PROC_RATE     "" CACHE STRING "Override: MML processing rate (Hz)")

foreach(OPTION
    MGC_MML_PSG_DEFAULT_LPF_ALPHA
    MGC_MML_PSG_LPF_SHIFT_BIT
    MGC_MML_PSG_EMULATOR_CLOCK
    MGC_MML_PSG_EMULATOR_RATE
    MGC_MML_PSG_MML_PROC_RATE
)
    if(NOT "${${OPTION}}" STREQUAL "")
        target_compile_definitions(mgc_sound_mml_psg PUBLIC ${OPTION}=${${OPTION}})
    endif()
endforeach()

if(MGC_USE_RP2040)
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
endif()

target_include_directories(mgc_sound_mml_psg PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_SRC_DIR}
    ${MGC_DRIVERS_DIR}
    ${MGC_EXTERNAL_DIR}/sound/emu2149
    ${MGC_EXTERNAL_DIR}/sound/Psgino/src
)

if(MGC_ENABLE_CPP_INTERFACE)
    target_include_directories(mgc_sound_mml_psg PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp
    )
endif()

add_subdirectory(${MGC_EXTERNAL_DIR}/sound/Psgino psgino_build)

if(MGC_USE_RP2040)
    target_link_libraries(mgc_sound_mml_psg 
        pico_stdlib
        hardware_pwm
        hardware_irq
        Psgino
    )
endif()

