cmake_minimum_required(VERSION 3.14)

project(scene CXX)

add_library(${PROJECT_NAME} STATIC
    scene_factory.cpp
    scene_core.cpp
    scene_base.cpp
    talkflow_effects.cpp
)

add_subdirectory(scenes/scene_title)
add_subdirectory(scenes/scene_password)
add_subdirectory(scenes/scene_tower_front)
add_subdirectory(scenes/scene_corridor)
add_subdirectory(scenes/scene_stage_1_1)
add_subdirectory(scenes/scene_stage_1_2)
add_subdirectory(scenes/scene_stage_1_3)
add_subdirectory(scenes/scene_stage_1_4)
add_subdirectory(scenes/scene_stage_1_5)
add_subdirectory(scenes/scene_stage_1_6)
add_subdirectory(scenes/scene_stage_1_7)
add_subdirectory(scenes/scene_stage_2_1)
add_subdirectory(scenes/scene_stage_2_2)
add_subdirectory(scenes/scene_stage_2_3)
add_subdirectory(scenes/scene_stage_2_4)
add_subdirectory(scenes/scene_stage_2_5)
add_subdirectory(scenes/scene_stage_2_6)
add_subdirectory(scenes/scene_stage_2_7)
add_subdirectory(scenes/scene_stage_2_8)
add_subdirectory(scenes/scene_stage_2_9)
add_subdirectory(scenes/scene_shop)
add_subdirectory(scenes/scene_log_terminal)
add_subdirectory(scenes/scene_game_over)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    platform
    resources
    scene_title
    scene_password
    scene_tower_front
    scene_corridor
    scene_stage_1_1
    scene_stage_1_2
    scene_stage_1_3
    scene_stage_1_4
    scene_stage_1_5
    scene_stage_1_6
    scene_stage_1_7
    scene_stage_2_1
    scene_stage_2_2
    scene_stage_2_3
    scene_stage_2_4
    scene_stage_2_5
    scene_stage_2_6
    scene_stage_2_7
    scene_stage_2_8
    scene_stage_2_9
    scene_shop
    scene_log_terminal
    scene_game_over
)

if (SIM_BUILD)
    target_compile_definitions(${PROJECT_NAME} PRIVATE MGC_SCENE_POOL_SIZE=16384)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE MGC_SCENE_POOL_SIZE=8192)
endif()


