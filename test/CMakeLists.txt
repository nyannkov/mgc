cmake_minimum_required(VERSION 3.14)

project(mgc_test LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MGC_C_COMPILE_FLAGS "-Wall -Wextra -Werror -O2" CACHE STRING "Compile flags for mgc_c library")
separate_arguments(MGC_C_COMPILE_FLAGS_LIST UNIX_COMMAND "${MGC_C_COMPILE_FLAGS}")
set(MGC_CPP_COMPILE_FLAGS "-Wall -Wextra -Werror -O2" CACHE STRING "Compile flags for mgc_cpp library")
separate_arguments(MGC_CPP_COMPILE_FLAGS_LIST UNIX_COMMAND "${MGC_CPP_COMPILE_FLAGS}")


set(MGC_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src" CACHE PATH "Path to mgc/src")
set(MGC_DRIVERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../drivers" CACHE PATH "Path to mgc/drivers")
set(MGC_EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../external" CACHE PATH "Path to mgc/external")

set(MGC_ENABLE_CPP_INTERFACE ON)

include_directories(
    ${MGC_EXTERNAL_DIR}/test/catch2
    ${MGC_SRC_DIR}
)

add_subdirectory(${MGC_EXTERNAL_DIR}/test/catch2 catch2_build)
add_subdirectory(${MGC_SRC_DIR}/mgc mgc_c_build)
add_subdirectory(${MGC_SRC_DIR}/mgc_cpp mgc_cpp_build)

enable_testing()
add_subdirectory(mgc_cpp)

