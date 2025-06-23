set(MGC_CPP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/parts/basic_dialoguebox.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/parts/basic_label.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/parts/basic_selectbox.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/parts/basic_sprite.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/parts/basic_tilegrid.cpp"
)

add_library(mgc_cpp STATIC
    ${MGC_CPP_SOURCES}
)

target_link_libraries(mgc_cpp PUBLIC mgc_c)

target_include_directories(mgc_cpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_compile_options(mgc_cpp PRIVATE ${MGC_CPP_COMPILE_FLAGS_LIST})

