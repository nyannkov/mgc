cmake_minimum_required(VERSION 3.14)

project(${EXEC_NAME} LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MGC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../" CACHE PATH "Path to MGC root directory")
message(STATUS "MGC_DIR=${MGC_DIR}")
set(TEST_GAME_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Path to test_game root directory")

add_library(app_headers INTERFACE)
target_include_directories(app_headers INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(CXX_SOURCES 
    main.cpp
    app_common.cpp
    utils/game_over_effect/game_over_effect.cpp
    utils/screen_fader/screen_fader.cpp
    status/status.cpp
    menu/equipment_menu.cpp
    entity/player/player.cpp
    entity/block/pushable_block/pushable_block.cpp
    entity/block/breakable_block/breakable_block.cpp
    entity/prop/gate/gate.cpp
    entity/prop/door/door.cpp
    entity/prop/portal/portal.cpp
    entity/prop/entrance/entrance.cpp
    entity/prop/signboard/signboard.cpp
    entity/prop/teleporter/teleporter.cpp
    entity/prop/button/button.cpp
    entity/prop/jewel/jewel.cpp
    entity/event/warp/warp.cpp
    entity/event/teleport/teleport.cpp
    entity/attack/attack_player/attack_player.cpp
    entity/attack/attack_lance/attack_lance.cpp
    entity/item/potion/potion.cpp
    entity/item/elixir/elixir.cpp
    entity/enemy/skyfish/skyfish.cpp
    entity/enemy/skyfish/skyfish_bt.cpp
    entity/enemy/walker/walker.cpp
    entity/enemy/floaty_fish/floaty_fish.cpp
    entity/enemy/balloon/balloon.cpp
    entity/enemy/boss/dancer/dancer.cpp
    entity/enemy/boss/lancer/lancer.cpp
    entity/stage/stage.cpp
    entity/stage/layer/tile_draw_hook.cpp
    entity/civilian/hen/hen.cpp
    entity/civilian/florist/florist.cpp
    entity/civilian/chick/chick.cpp
)


add_executable(${CMAKE_PROJECT_NAME}
    ${CXX_SOURCES}
)

add_subdirectory("${MGC_DIR}"  mgc_build)
add_subdirectory(scene)
add_subdirectory(platform)
add_subdirectory(resources)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    platform
    resources
    scene
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(MY_CUSTOM_CONF "\"${CMAKE_CURRENT_SOURCE_DIR}/mgc_conf.h\"")
target_compile_definitions(mgc_c PUBLIC MGC_CUSTOM_CONF_PATH=${MY_CUSTOM_CONF})

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
    -fno-exceptions
)

if(NOT SIM_BUILD)
    pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
endif()

