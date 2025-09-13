cmake_minimum_required(VERSION 3.14)

project(mml C)

add_library(${PROJECT_NAME} STATIC
    mml.c
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    mgc
)

