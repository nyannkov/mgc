cmake_minimum_required(VERSION 3.12)
project(scene_000_title CXX)

add_library(${PROJECT_NAME} STATIC
    scene_000.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
