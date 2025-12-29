set(MML_PSG_SRC
    "mml_psg.cpp"
    "${MGC_EXTERNAL_DIR}/sound/emu2149/emu2149.c" # Specify source files directly to bypass the -O3 optimization in emu2149's CMakeLists.txt
)

if(MGC_USE_RP2040)
    add_subdirectory("ports/rp2040")
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

target_link_libraries(mgc_sound_mml_psg
    Psgino
)

target_link_libraries(mgc_sound_mml_psg
    mml_psg_port
)

