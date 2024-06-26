cmake_minimum_required(VERSION 3.16)

include("$ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake")

project(${CMAKE_PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
endif()

pico_sdk_init()

set(MGC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)
set(MGC_SRC_PATH ${MGC_PATH}/src)
set(MGC_DEVICES_PATH ${MGC_PATH}/devices)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    system/system.c
    system/sound_list.c
    screen/main/scenes/scene_001.cpp
    screen/main/scenes/scene_002.cpp
    screen/main/scenes/scenes.cpp
    screen/sub/dlgflow_handler_sub_screen.c
    screen/sub/scenes/scene_sub.cpp
    actor/actor_if.cpp
    actor/static_object.cpp
    actor/player.cpp
    anim/anim_player.cpp
)

add_subdirectory(resources)
add_subdirectory(${MGC_PATH} mgc)

# sound driver
set(MGC_SOUND_PSG_TYPE ymz294)
#set(MGC_SOUND_PSG_TYPE emulator)
add_subdirectory(${MGC_DEVICES_PATH}/mgc/sound/psg sound_psg)

# gamepad driver
add_subdirectory(${MGC_DEVICES_PATH}/mgc/gamepad/sample gamepad_sample)

# display driver
set(MGC_DISPLAY_LCD_TYPE ili9341)
add_subdirectory(${MGC_DEVICES_PATH}/mgc/display/lcd display_lcd)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    pico_stdlib
    mgc
    sound_psg
    gamepad_sample
    display_lcd
    resources_gen
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

