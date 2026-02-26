cmake_minimum_required(VERSION 3.12)
project(scene_password CXX)

add_library(${PROJECT_NAME} STATIC
    scene_password.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
    scene
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
