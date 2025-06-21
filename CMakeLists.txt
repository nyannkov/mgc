cmake_minimum_required(VERSION 3.14)
project(mgc LANGUAGES C CXX)

option(BUILD_MGC_CPP "Build mgc_cpp C++ library" ON)
option(BUILD_TESTING "Build tests" ON)

add_subdirectory(src/mgc)

if(BUILD_MGC_CPP)
    add_subdirectory(src/mgc_cpp)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()


add_library(mgc INTERFACE)

target_link_libraries(mgc INTERFACE mgc_c)

if(BUILD_MGC_CPP)
    target_link_libraries(mgc INTERFACE mgc_cpp)
endif()

