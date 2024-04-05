cmake_minimum_required(VERSION 3.16)

add_library(sound_psg STATIC sound_psg.c)

target_include_directories(sound_psg PRIVATE
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

if( MGC_SOUND_PSG_TYPE STREQUAL "ymz294" )
    target_compile_definitions(sound_psg PRIVATE MGC_SOUND_PSG_TYPE=1)
    add_subdirectory(ymz294)
    target_link_libraries(sound_psg sound_psg_ymz294)

elseif( MGC_SOUND_PSG_TYPE STREQUAL "emulator" )
    target_compile_definitions(sound_psg PRIVATE MGC_SOUND_PSG_TYPE=10)
    add_subdirectory(emulator)
    target_link_libraries(sound_psg sound_psg_emulator)

else()
    message(FATAL_ERROR "No sound-psg selected.")

endif()

