add_executable(test_math 
    test_vec2.cpp
)
target_link_libraries(test_math PRIVATE mgc Catch2::Catch2WithMain)
add_test(NAME test_math COMMAND test_math)

