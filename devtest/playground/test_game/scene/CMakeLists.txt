cmake_minimum_required(VERSION 3.14)

project(scene CXX)

add_library(${PROJECT_NAME} STATIC
    scene.cpp
)

add_subdirectory(scenes/scene_000_title)
add_subdirectory(scenes/scene_001_tower)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    mgc
    scene_000_title
    scene_001_tower
)

