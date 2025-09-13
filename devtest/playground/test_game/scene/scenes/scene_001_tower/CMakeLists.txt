cmake_minimum_required(VERSION 3.12)
project(scene_001_tower CXX)

add_library(${PROJECT_NAME} STATIC
    scene_001.cpp
    enemies/scene_001_enemies.cpp
    items/scene_001_items.cpp
    props/scene_001_props.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
