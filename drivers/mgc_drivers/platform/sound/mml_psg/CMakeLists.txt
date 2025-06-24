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


set(MGC_MML_PSG_RP2040_ALARM_NUM "" CACHE STRING "Override: Alarm number for MML PSG")
set(MGC_MML_PSG_RP2040_ALARM_IRQ_PRI "" CACHE STRING "Override: Alarm IRQ priority")
set(MGC_MML_PSG_RP2040_PWM_IRQ_PRI "" CACHE STRING "Override: PWM IRQ priority")
set(MGC_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT "" CACHE STRING "Override: GPIO pin for PSG output")

if(NOT "${MGC_MML_PSG_RP2040_ALARM_NUM}" STREQUAL "")
	target_compile_definitions(mgc_drivers_mml_psg PUBLIC
		MGC_DRIVERS_MML_PSG_RP2040_ALARM_NUM=${MGC_MML_PSG_RP2040_ALARM_NUM})
endif()
if(NOT "${MGC_MML_PSG_RP2040_ALARM_IRQ_PRI}" STREQUAL "")
	target_compile_definitions(mgc_drivers_mml_psg PUBLIC
		MGC_DRIVERS_MML_PSG_RP2040_ALARM_IRQ_PRI=${MGC_MML_PSG_RP2040_ALARM_IRQ_PRI})
endif()
if(NOT "${MGC_MML_PSG_RP2040_PWM_IRQ_PRI}" STREQUAL "")
	target_compile_definitions(mgc_drivers_mml_psg PUBLIC
		MGC_DRIVERS_MML_PSG_RP2040_PWM_IRQ_PRI=${MGC_MML_PSG_RP2040_PWM_IRQ_PRI})
endif()
if(NOT "${MGC_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT}" STREQUAL "")
	target_compile_definitions(mgc_drivers_mml_psg PUBLIC
		MGC_DRIVERS_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT=${MGC_MML_PSG_RP2040_PIN_PSG_EMU_OUTPUT})
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

