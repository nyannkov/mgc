add_library(mgc_drivers INTERFACE)

option(MGC_ENABLE_DISPLAY "Enable display drivers" ON)
option(MGC_ENABLE_SOUND "Enable sound drivers" ON)
option(MGC_ENABLE_INPUT "Enable input drivers" ON)

if(MGC_USE_RP2040)
    target_compile_definitions(mgc_drivers INTERFACE MGC_PLATFORM_RP2040)
endif()

# === Device Selection with GUI-friendly STRINGS ===
set(MGC_DISPLAY_DEVICE "st7789" CACHE STRING "Display device to use")
set_property(CACHE MGC_DISPLAY_DEVICE PROPERTY STRINGS "st7789")

set(MGC_INPUT_DEVICE "digital_gamepad" CACHE STRING "Input device to use")
set_property(CACHE MGC_INPUT_DEVICE PROPERTY STRINGS "digital_gamepad")

set(MGC_SOUND_DEVICE "mml_psg" CACHE STRING "Sound device to use")
set_property(CACHE MGC_SOUND_DEVICE PROPERTY STRINGS "mml_psg")

add_subdirectory(platform)

