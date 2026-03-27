cmake_minimum_required(VERSION 3.12)
project(scene_shop CXX)

add_library(${PROJECT_NAME} STATIC
    scene_shop.cpp
    event_objects/event_shop_1.cpp
    event_objects/event_shop_2.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    mgc
    app_headers
    scene
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
