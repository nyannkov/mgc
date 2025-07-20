cmake_minimum_required(VERSION 3.14)

project(playground.elf LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(C_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_map_elements.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_player.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_blue.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_fish.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_items.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/talkscript/test_talkscript.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/font/k8x12.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/map/map_main_layer_01.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/map/map_bg_layer_01.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/btree/test_btree.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/btree/btree_chase.c"
)

add_executable(${CMAKE_PROJECT_NAME}
    ${C_SOURCES}
    )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../..  mgc_build)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    mgc
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
    -fno-exceptions
)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

