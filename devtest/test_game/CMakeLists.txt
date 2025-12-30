cmake_minimum_required(VERSION 3.14)

project(${EXEC_NAME} LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(app_headers INTERFACE)
target_include_directories(app_headers INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_library(mgc_appconf INTERFACE)
target_link_libraries(mgc_appconf INTERFACE mgc)

set(CXX_SOURCES 
    main.cpp
    app_common.cpp
    game_context/default_talkflow_effects.cpp
    utils/game_over_effect/game_over_effect.cpp
    status/status.cpp
    entity/player/player.cpp
    entity/prop/gate/gate.cpp
    entity/prop/entrance/entrance.cpp
    entity/prop/signboard/signboard.cpp
    entity/attack/scratch/scratch.cpp
    entity/item/potion/potion.cpp
    entity/item/elixir/elixir.cpp
    entity/enemy/skyfish/skyfish.cpp
    entity/enemy/skyfish/skyfish_bt.cpp
    entity/stage/stage.cpp
    entity/stage/layer/tile_draw_hook.cpp
    entity/civilian/hen/hen.cpp
    entity/civilian/chick/chick.cpp
)

#TODO
if (SIM_BUILD)
    list(APPEND CXX_SOURCES
        platform/simulation/drivers/ports/free_running_timer_sim.cpp
    )
endif()

set(C_SOURCES 
    resources/generated/tileset/tileset_map_elements.c
    resources/generated/tileset/tileset_items.c
    resources/generated/tileset/tileset_attack.c
    resources/generated/tileset/tileset_hen.c
    resources/generated/tileset/tileset_chick.c
    resources/generated/tileset/tileset_static_objects.c
    resources/generated/tileset/tileset_static_objects_16x48.c
    resources/generated/tileset/tileset_light_rays.c
    resources/generated/map/map_tower_front_bg_back.c
    resources/generated/map/map_tower_front_back_tower.c
    resources/generated/map/map_tower_front_block.c
    resources/generated/map/map_corridor_back.c
    resources/generated/map/map_corridor_block.c
    resources/generated/map/map_stage_1_1_block.c
    resources/generated/map/map_stage_1_1_ladder.c
    resources/generated/map/map_stage_1_1_one_way_block.c
    resources/generated/btree/btree_chase.c
    resources/generated/font/k8x12.c
    resources/generated/font/misaki_gothic.c
    resources/generated/talkscript/talkscript_1.c
)

add_executable(${CMAKE_PROJECT_NAME}
    ${C_SOURCES}
    ${CXX_SOURCES}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../  mgc_build)
add_subdirectory(resources/generated/anim/player)
add_subdirectory(resources/generated/anim/enemy/skyfish)
add_subdirectory(resources/mml)
add_subdirectory(scene)

add_subdirectory(platform)
target_link_libraries(mgc_appconf INTERFACE platform)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    mgc_appconf
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

if(NOT SIM_BUILD)
    pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
endif()

