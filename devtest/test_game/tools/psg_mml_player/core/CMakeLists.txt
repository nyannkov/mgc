cmake_minimum_required(VERSION 3.28)

project(${EXEC_NAME} LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MGC_DIR "${MGC_ABS_PATH}" CACHE PATH "Path to MGC root directory")
message(STATUS "MGC_DIR=${MGC_DIR}")

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)

add_subdirectory("${MGC_DIR}"  mgc_build)
add_subdirectory(../../../platform/simulation/drivers/free_running_timer/ports/ free_running_timer_port)
add_subdirectory(../../../platform/simulation/drivers/mml_psg/ports/ mml_psg_port)

target_link_libraries(${CMAKE_PROJECT_NAME}
    mgc
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
    -fno-exceptions
)

