cmake_minimum_required(VERSION 3.16)

file(GLOB FONT_SRC generates/font/*.c)
file(GLOB TILESET_SRC generates/tileset/*.c)
file(GLOB MAP_SRC generates/map/*.c)

add_library(sys_resources_gen STATIC
    ${TILESET_SRC}
    ${MAP_SRC}
    ${FONT_SRC}
)

target_include_directories(sys_resources_gen PRIVATE
    ${MGC_SRC_PATH}
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
)
