add_library(display_sdl2 INTERFACE)

find_package(SDL2 REQUIRED)

target_include_directories(display_sdl2 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIR} 
)

