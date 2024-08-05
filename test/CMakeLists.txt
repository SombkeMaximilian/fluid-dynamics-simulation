cmake_minimum_required(VERSION 3.25)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_FILES
    test_grid.cpp
    test_bound.cpp
    test_solver.cpp
    test_utils.h
)

add_executable(FDSimUnitTests ${TEST_FILES})
target_link_libraries(FDSimUnitTests gtest gtest_main)

add_test(NAME FDSimUnitTests COMMAND FDSimUnitTests)
