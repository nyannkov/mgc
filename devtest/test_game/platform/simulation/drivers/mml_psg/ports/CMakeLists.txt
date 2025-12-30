add_library(mml_psg_port STATIC
    mml_psg_sdl2.c
)

find_package(SDL2 REQUIRED)

target_link_libraries(mml_psg_port
    mgc_sound_mml_psg
    ${SDL2_LIBRARIES}
)

