cmake_minimum_required(VERSION 3.14)

project(playground.elf LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(app_headers INTERFACE)
target_include_directories(app_headers INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(CXX_SOURCES 
    main.cpp
    status/status.cpp
    scene/scene.cpp
    entity/player/player.cpp
    entity/prop/entrance/entrance.cpp
    entity/attack/scratch/scratch.cpp
    entity/item/potion/potion.cpp
    entity/item/elixir/elixir.cpp
    entity/enemy/skyfish/skyfish.cpp
    entity/enemy/skyfish/skyfish_bt.cpp
    entity/stage/collision_tile_layer/collision_tile_layer.cpp
    entity/stage/collision_tile_layer/collision_tile_index_hook.cpp
    entity/stage/tile_layer/tile_layer.cpp
    entity/stage/tile_layer/tile_index_hook.cpp
)

set(C_SOURCES 
    resources/generated/tileset/tileset_map_elements.c
    resources/generated/tileset/tileset_items.c
    resources/generated/tileset/tileset_attack.c
    resources/generated/tileset/tileset_static_objects.c
    resources/generated/map/map_scene_001_main.c
    resources/generated/map/map_scene_001_bg1.c
    resources/generated/map/map_scene_001_bg2.c
    resources/generated/btree/btree_chase.c
    resources/generated/font/k8x12.c
    resources/generated/font/misaki_gothic.c
    resources/generated/talkscript/test_talkscript.c
)

add_executable(${CMAKE_PROJECT_NAME}
    ${C_SOURCES}
    ${CXX_SOURCES}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../..  mgc_build)
add_subdirectory(resources/generated/anim/player)
add_subdirectory(resources/generated/anim/enemy/skyfish)
add_subdirectory(resources/mml)
add_subdirectory(scene)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    mgc
    anim_player
    anim_skyfish
    mml
    scene
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

