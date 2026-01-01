add_library(mgc_timer_free_running_timer STATIC
    free_running_timer.c
)

if(MGC_USE_RP2040)
    add_subdirectory(ports/rp2040)
endif()

target_include_directories(mgc_timer_free_running_timer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
    ${MGC_SRC_DIR}
    ${MGC_DRIVERS_DIR}
)

target_link_libraries(mgc_timer_free_running_timer 
    free_running_timer_port
)

