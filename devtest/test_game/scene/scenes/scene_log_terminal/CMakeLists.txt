cmake_minimum_required(VERSION 3.12)
project(scene_log_terminal CXX)

add_library(${PROJECT_NAME} STATIC
    scene_log_terminal.cpp
    event_objects/event_log_terminal_1.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
    scene
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
