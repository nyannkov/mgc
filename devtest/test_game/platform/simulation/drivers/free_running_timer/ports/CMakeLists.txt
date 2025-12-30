add_library(free_running_timer_port OBJECT
    free_running_timer_chrono.cpp
)

target_link_libraries(free_running_timer_port
    mgc
)
