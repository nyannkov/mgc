add_library(mml_psg_port STATIC
    mml_psg_sdl2.c
)

find_package(SDL2 REQUIRED)

target_include_directories(mml_psg_port PUBLIC
    ${MGC_DIR}/src
    ${MGC_DIR}/drivers/mgc_drivers/platform/sound/mml_psg
)

target_link_libraries(mml_psg_port
    ${SDL2_LIBRARIES}
)

