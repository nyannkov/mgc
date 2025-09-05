cmake_minimum_required(VERSION 3.14)

project(playground.elf LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(C_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/status/status.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/scene/scene.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity/player/player.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity/effect/effect.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity/enemy/skyfish/skyfish.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity/enemy/skyfish/skyfish_bt.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity/stage/collision_tile_layer/collision_tile_layer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity/stage/collision_tile_layer/collision_tile_index_hook.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity/stage/tile_layer/tile_layer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity/stage/tile_layer/tile_index_hook.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_map_elements.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_items.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/tileset/tileset_effect.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/map/map_main_layer_01.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/map/map_bg_layer_01.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/btree/btree_chase.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/font/k8x12.c"
)

add_executable(${CMAKE_PROJECT_NAME}
    ${C_SOURCES}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../..  mgc_build)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/anim/player)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/anim/enemy/skyfish)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    mgc
    anim_player
    anim_skyfish
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

