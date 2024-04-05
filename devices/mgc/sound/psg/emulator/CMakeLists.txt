cmake_minimum_required(VERSION 3.16)

add_library(sound_psg_emulator INTERFACE)

add_subdirectory(rp2040)
target_link_libraries(sound_psg_emulator INTERFACE sound_psg_emulator_rp2040)

