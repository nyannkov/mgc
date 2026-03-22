add_library(gamepad_sdl2 STATIC
    gamepad_sdl2.c
)

find_package(SDL2 REQUIRED)

target_include_directories(gamepad_sdl2
PUBLIC
    ${MGC_DIR}/src
    ${MGC_DIR}/drivers/mgc_drivers/platform/input/gamepad/core
    ${MGC_DIR}/drivers/mgc_drivers/platform/input/gamepad/core/cpp
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp

PRIVATE
    ${SDL2_INCLUDE_DIR} 
)

target_link_libraries(gamepad_sdl2
    ${SDL2_LIBRARIES}
)

