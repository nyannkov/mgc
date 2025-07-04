set(FREE_RUNNING_TIMER_SRC
    "free_running_timer.c"
)

if(MGC_USE_RP2040)
    list(APPEND FREE_RUNNING_TIMER_SRC
        "ports/rp2040/free_running_timer_rp2040.c"
    )
endif()

add_library(mgc_timer_free_running_timer STATIC ${FREE_RUNNING_TIMER_SRC})


target_include_directories(mgc_timer_free_running_timer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MGC_SRC_DIR}
    ${MGC_DRIVERS_DIR}
)

if(MGC_ENABLE_CPP_INTERFACE)
    target_include_directories(mgc_timer_free_running_timer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp
    )
endif()

if(MGC_USE_RP2040)
    target_link_libraries(mgc_timer_free_running_timer 
        pico_time
    )
endif()

