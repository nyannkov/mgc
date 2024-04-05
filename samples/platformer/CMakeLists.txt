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
    ${MGC_DEVICES_PATH}/mgc/gamepad/rp2040/gamepad_rp2040.c
    ${MGC_DEVICES_PATH}/mgc/display/ili9341/rp2040/display_ili9341_rp2040.c
)

add_subdirectory(resources)
add_subdirectory(${MGC_PATH} mgc)
add_subdirectory(${MGC_DEVICES_PATH}/mgc/sound/psg/ymz294 sound_psg_ymz294)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    mgc
    sound_psg_ymz294
    pico_stdlib
    hardware_spi
    hardware_gpio
    hardware_adc
    hardware_pwm
    hardware_irq
    resources_gen
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

