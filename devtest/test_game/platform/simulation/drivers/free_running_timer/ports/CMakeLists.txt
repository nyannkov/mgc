add_library(free_running_timer_port STATIC
    free_running_timer_chrono.cpp
)

target_include_directories(free_running_timer_port PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_DIR}/src
    ${MGC_DIR}/drivers/mgc_drivers/platform/timer/free_running_timer/ports
)

