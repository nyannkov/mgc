add_executable(test_graphics
    test_framebuffer.cpp
    test_cell_buffer.cpp
)
target_link_libraries(test_graphics PRIVATE mgc_cpp Catch2::Catch2WithMain)
add_test(NAME test_graphics COMMAND test_graphics)

