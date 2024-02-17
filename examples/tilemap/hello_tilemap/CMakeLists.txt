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

set(MGC_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../src)
set(MGC_DEVICES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../devices)

add_executable(${CMAKE_PROJECT_NAME}
    main.c
    system/system_rp2040.c
    ${MGC_DEVICES_PATH}/mgc/display/ili9341/rp2040/display_ili9341_rp2040.c
)

add_subdirectory(resources)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../ build)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    mgc
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

