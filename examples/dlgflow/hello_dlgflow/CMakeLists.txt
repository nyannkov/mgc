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

set(MGC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
set(MGC_SRC_PATH ${MGC_PATH}/src)
set(MGC_DEVICES_PATH ${MGC_PATH}/devices)
set(MGC_EXAMPLES_COMMON_PATH ${MGC_PATH}/examples/common)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)

add_subdirectory(${MGC_PATH} mgc)
add_subdirectory(${MGC_EXAMPLES_COMMON_PATH}/system system)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_SRC_PATH}
    ${MGC_DEVICES_PATH}
    ${MGC_EXAMPLES_COMMON_PATH}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    mgc
    system
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

