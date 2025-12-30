
add_library(platform STATIC
   platform.cpp 
)

add_library(platform_bundle INTERFACE)

if (SIM_BUILD)
    target_compile_definitions(platform PUBLIC MGC_SIMULATION)

    add_subdirectory(simulation/drivers/gamepad_sdl2)
    target_link_libraries(platform_bundle INTERFACE gamepad_sdl2)

    add_subdirectory(simulation/drivers/display_sdl2)
    target_link_libraries(platform_bundle INTERFACE display_sdl2)

    add_subdirectory(simulation/drivers/mml_psg/ports)

#    add_subdirectory(simulation/drivers/free_running_timer/ports)
endif()

target_link_libraries(platform
    platform_bundle
    mgc
)

