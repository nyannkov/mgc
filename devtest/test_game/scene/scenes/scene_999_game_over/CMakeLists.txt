cmake_minimum_required(VERSION 3.12)
project(scene_999_game_over CXX)

add_library(${PROJECT_NAME} STATIC
    scene_999_game_over.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
    scene
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
