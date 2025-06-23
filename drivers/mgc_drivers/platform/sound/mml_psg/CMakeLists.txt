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

