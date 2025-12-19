cmake_minimum_required(VERSION 3.12)
project(scene_001_tower_front CXX)

add_library(${PROJECT_NAME} STATIC
    scene_001_tower_front.cpp
    event_objects/event_tower_front_1.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
