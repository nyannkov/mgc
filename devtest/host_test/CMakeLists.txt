cmake_minimum_required(VERSION 3.14)

project(host_test.out LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(C_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/generated/btree/test_btree.c"
)

add_executable(${CMAKE_PROJECT_NAME}
    ${C_SOURCES}
    )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../..  mgc_build)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
    mgc
)

add_compile_options(-Wall
    -Wno-format
    -Wno-unused-function
    -Wno-maybe-uninitialized
)

