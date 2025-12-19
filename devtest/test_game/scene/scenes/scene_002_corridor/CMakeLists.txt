cmake_minimum_required(VERSION 3.12)
project(scene_002_corridor CXX)

add_library(${PROJECT_NAME} STATIC
    scene_002_corridor.cpp
    event_objects/event_corridor_1.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
