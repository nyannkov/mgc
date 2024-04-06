cmake_minimum_required(VERSION 3.16)

add_library(gamepad_sample INTERFACE)

add_subdirectory(rp2040)
target_link_libraries(gamepad_sample INTERFACE gamepad_sample_rp2040)

