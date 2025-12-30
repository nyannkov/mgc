
if (MGC_ENABLE_DISPLAY)
    if(MGC_DISPLAY_DEVICE STREQUAL "st7789")
        add_subdirectory(display/st7789)
        target_link_libraries(mgc_drivers INTERFACE mgc_display_st7789)
    else()
        message(FATAL_ERROR "Unknown display device: ${MGC_DISPLAY_DEVICE}")
    endif()  
endif()

if(MGC_ENABLE_SOUND)
    if(MGC_SOUND_DEVICE STREQUAL "mml_psg")
        add_subdirectory(sound/mml_psg)
        target_link_libraries(mgc_drivers INTERFACE mgc_sound_mml_psg)
    else()
        message(FATAL_ERROR "Unknown sound device: ${MGC_SOUND_DEVICE}")
    endif()
endif()

if(MGC_ENABLE_INPUT)
    if(MGC_INPUT_DEVICE STREQUAL "gamepad")

        add_library(mgc_gamepad_bundle INTERFACE)

        add_subdirectory(input/gamepad/core)
        target_link_libraries(mgc_gamepad_bundle INTERFACE
            mgc_input_gamepad
        )

        if(MGC_USE_RP2040)
            add_subdirectory(input/gamepad/source/picolcd)
            target_link_libraries(mgc_gamepad_bundle INTERFACE
                gamepad_picolcd
            )
        endif()

        target_link_libraries(mgc_drivers INTERFACE mgc_gamepad_bundle)

    else()
        message(FATAL_ERROR "Unknown input device: ${MGC_INPUT_DEVICE}")
    endif()
endif()

if(MGC_ENABLE_TIMER)
    add_subdirectory(timer/free_running_timer)
    target_link_libraries(mgc_drivers INTERFACE mgc_timer_free_running_timer)
endif()

