add_library(free_running_timer_port STATIC
    free_running_timer_rp2040.c
)

target_include_directories(free_running_timer_port PRIVATE
    ${MGC_SRC_DIR}
    ${MGC_DRIVERS_DIR}/mgc_drivers/platform/timer/free_running_timer
)

target_link_libraries(free_running_timer_port
    pico_time
)

