cmake_minimum_required(VERSION 3.27)

enable_testing()

add_executable(runTests test_grid.cpp)
target_link_libraries(runTests gtest gtest_main)

add_test(NAME runTests COMMAND runTests)
