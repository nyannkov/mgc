cmake_minimum_required(VERSION 3.12)
project(scene_stage_3_3 CXX)

add_library(${PROJECT_NAME} STATIC
    scene_stage_3_3.cpp
    scene_objects/scene_objects_stage_3_3.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
    scene
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
