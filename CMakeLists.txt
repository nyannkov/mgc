cmake_minimum_required(VERSION 3.14)

project(mgc LANGUAGES C CXX ASM)

# === Language Standards ===
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Compile Flags ===
set(MGC_C_COMPILE_FLAGS "-Wall -Wextra -Werror -O2" CACHE STRING "Compile flags for mgc_c library")
separate_arguments(MGC_C_COMPILE_FLAGS_LIST UNIX_COMMAND "${MGC_C_COMPILE_FLAGS}")
set(MGC_CPP_COMPILE_FLAGS "-Wall -Wextra -Werror -O2" CACHE STRING "Compile flags for mgc_cpp library")
separate_arguments(MGC_CPP_COMPILE_FLAGS_LIST UNIX_COMMAND "${MGC_CPP_COMPILE_FLAGS}")

# === Directory Definitions ===
set(MGC_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(MGC_DRIVERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/drivers")
set(MGC_EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")


# === Platform Support Options ===
option(MGC_USE_RP2040 "Enable RP2040 platform support" ON)

if(MGC_USE_RP2040)
    # Set and import pico-sdk if RP2040 is used
    set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}" CACHE PATH "Path to pico-sdk")
    include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
    pico_sdk_init()
endif()

# === Build Options ===
option(MGC_ENABLE_CPP_INTERFACE "Build C++ interface layer (src/mgc_cpp)" ON)
option(MGC_ENABLE_DRIVERS "Enable platform driver modules (display, sound, input)" ON)

# === Core Modules ===

# Always build the C core library
add_subdirectory(src/mgc)

# Build C++ interface library (optional)
if(MGC_ENABLE_CPP_INTERFACE)
    add_subdirectory(src/mgc_cpp)
endif()

# Build platform driver libraries (optional)
if(MGC_ENABLE_DRIVERS)
    add_subdirectory(drivers)
endif()


# Define a top-level interface target that includes selected components
add_library(mgc INTERFACE)


# Link driver bundle if enabled
if(MGC_ENABLE_DRIVERS)
    target_link_libraries(mgc INTERFACE mgc_drivers)
endif()

# Link the C core library
target_link_libraries(mgc INTERFACE mgc_c)

# Link the C++ interface if enabled
if(MGC_ENABLE_CPP_INTERFACE)
    target_link_libraries(mgc INTERFACE mgc_cpp)
endif()

