cmake_minimum_required(VERSION 3.14)

project(playground.elf LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(C_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_map_elements.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_player.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/map/map_01.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/map/map_01_fg.c"
)

add_executable(${CMAKE_PROJECT_NAME}
    ${C_SOURCES}
    )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../..  mgc_build)


target_include_directories(playground.elf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../mgc/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../mgc/drivers
)

target_link_libraries(playground.elf PUBLIC
    mgc
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

