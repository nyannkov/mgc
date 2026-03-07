cmake_minimum_required(VERSION 3.14)

project(scene CXX)

add_library(${PROJECT_NAME} STATIC
    scene_factory.cpp
    scene_core.cpp
    scene_base.cpp
    default_talkflow_effects.cpp
)

add_subdirectory(scenes/scene_title)
add_subdirectory(scenes/scene_password)
add_subdirectory(scenes/scene_tower_front)
add_subdirectory(scenes/scene_corridor)
add_subdirectory(scenes/scene_stage_1_1)
add_subdirectory(scenes/scene_stage_1_2)
add_subdirectory(scenes/scene_stage_1_3)
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
    scene_game_over
)

if (SIM_BUILD)
    target_compile_definitions(${PROJECT_NAME} PRIVATE MGC_SCENE_POOL_SIZE=8192)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE MGC_SCENE_POOL_SIZE=4096)
endif()


