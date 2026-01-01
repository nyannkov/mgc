cmake_minimum_required(VERSION 3.14)

project(scene CXX)

add_library(${PROJECT_NAME} STATIC
    scene.cpp
    scene_helper.cpp
)

add_subdirectory(scenes/scene_000_title)
add_subdirectory(scenes/scene_001_tower_front)
add_subdirectory(scenes/scene_002_corridor)
add_subdirectory(scenes/scene_003_stage_1_1)
add_subdirectory(scenes/scene_999_game_over)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    platform
    scene_000_title
    scene_001_tower_front
    scene_002_corridor
    scene_003_stage_1_1
    scene_999_game_over
)

if (SIM_BUILD)
    target_compile_definitions(${PROJECT_NAME} PRIVATE MGC_SCENE_POOL_SIZE=8192)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE MGC_SCENE_POOL_SIZE=4096)
endif()


