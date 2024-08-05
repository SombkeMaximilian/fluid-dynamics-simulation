cmake_minimum_required(VERSION 3.25)

enable_testing()

set(TEST_FILES
    test_grid.cpp
    test_bound.cpp
    test_solver.cpp
    test_utils.h
)

add_executable(runTests ${TEST_FILES})
target_link_libraries(runTests gtest gtest_main)

add_test(NAME runTests COMMAND runTests)
