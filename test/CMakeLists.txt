add_subdirectory(${CMAKE_SOURCE_DIR}/external/test/catch2 ${CMAKE_BINARY_DIR}/external_test_catch2)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(mgc_cpp)

