cmake_minimum_required(VERSION 3.5)

project(sim_tests LANGUAGES CXX)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(decoder_tests decoder.cpp)
target_link_libraries(decoder_tests
    GTest::GTest
    GTest::Main
    sim 
)
